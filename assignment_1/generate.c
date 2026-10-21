#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"
#include "io.h"
#include "sort.h"

// Input file names
#define FIRST_NAMES_FILE    "first_names.txt"
#define LAST_NAMES_FILE     "last_names.txt"
#define COUNTRIES_FILE      "countries.txt"
#define EMAIL_SUFFIXES_FILE "email_suffixes.txt"

// Number of entries in each input file
#define MAX_FIRST_NAMES     1000
#define MAX_LAST_NAMES      1000
#define MAX_COUNTRIES       195
#define MAX_EMAIL_SUFFIXES  100

// Buffer sizes 
#define STRING_SIZE         64    // one line of an input file (must match io.c)
#define CELL_SIZE           256   // one generated value
#define ROW_SIZE            2048  // one complete CSV line
#define FULL_FILE_NAME_SIZE 68    // 63-char name + ".csv" + '\0'

// Data generation rules
#define NUM_COLUMNS         8
#define NUM_PHONE_INDEXES   10
#define PHONE_LINE_RANGE    10000 // last 4 digits: 0000 to 9999
#define SIN_LENGTH          9
#define MIN_PASSWORD_LENGTH 6
#define MAX_PASSWORD_LENGTH 16

// Data shared by the helper functions in this file
static char firstNames[MAX_FIRST_NAMES][STRING_SIZE];
static char lastNames[MAX_LAST_NAMES][STRING_SIZE];
static char countries[MAX_COUNTRIES][STRING_SIZE];
static char emailSuffixes[MAX_EMAIL_SUFFIXES][STRING_SIZE];

static const int phoneIndexes[NUM_PHONE_INDEXES] = {
    398, 270, 925, 867, 209,
    429, 908, 997, 444, 219
};

static const char allowedChars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!\"#$%&'()*+-./:;<=>?@[\\]^_`{|}~";

static int fNameCount = 0;
static int lNameCount = 0;
static int countryCount = 0;
static int emailSuffixCount = 0;


// Release allocated memory
static void freeRows(GeneratedRow *rows, int rowCount) {
    for (int i = 0; i < rowCount; i++) {
        free(rows[i].sortKey);
        free(rows[i].csvLine);
    }

    free(rows);
}


// Check the selected columns. Returns 1 if OK, 0 if not
static int validateInput(int rowCount, int numCols, int cols[]) {
    if (rowCount < 1 || numCols < 1) {
        printf("Invalid row count or column count.\n");
        return 0;
    }

    for (int i = 0; i < numCols; i++) {
        if (cols[i] < 1 || cols[i] > NUM_COLUMNS) {
            printf("Only columns 1 through %d are implemented.\n",
                   NUM_COLUMNS);
            return 0;
        }
    }

    return 1;
}


// Load only the files needed by the selected columns.
// Returns 1 if OK, 0 if a file could not be loaded.
static int loadFiles(int cols[], int numCols) {
    for (int i = 0; i < numCols; i++) {

        if ((cols[i] == 2 || cols[i] == 6) && fNameCount == 0) {
            fNameCount = loadStrings(
                FIRST_NAMES_FILE, firstNames, MAX_FIRST_NAMES);

            if (fNameCount <= 0) {
                printf("Could not load first names.\n");
                return 0;
            }
        }

        if ((cols[i] == 3 || cols[i] == 6) && lNameCount == 0) {
            lNameCount = loadStrings(
                LAST_NAMES_FILE, lastNames, MAX_LAST_NAMES);

            if (lNameCount <= 0) {
                printf("Could not load last names.\n");
                return 0;
            }
        }

        if (cols[i] == 4 && countryCount == 0) {
            countryCount = loadStrings(
                COUNTRIES_FILE, countries, MAX_COUNTRIES);

            if (countryCount <= 0) {
                printf("Could not load countries.\n");
                return 0;
            }
        }

        if (cols[i] == 6 && emailSuffixCount == 0) {
            emailSuffixCount = loadStrings(
                EMAIL_SUFFIXES_FILE, emailSuffixes, MAX_EMAIL_SUFFIXES);

            if (emailSuffixCount <= 0) {
                printf("Could not load email suffixes.\n");
                return 0;
            }
        }
    }

    return 1;
}


// Generate the value of one column for one row and store it in cell.
// (cellSize is passed in because sizeof(cell) would give the size of a
// pointer here, not the size of the array.)
static void makeCell(char *cell, size_t cellSize, int column, int row,
                     int firstNameIndex, int lastNameIndex) {

    if (column == 1) {
        // User ID is the row number
        snprintf(cell, cellSize, "%d", row);

    } else if (column == 2) {
        snprintf(cell, cellSize, "%s",
                 firstNames[firstNameIndex]);

    } else if (column == 3) {
        snprintf(cell, cellSize, "%s",
                 lastNames[lastNameIndex]);

    } else if (column == 4) {
        int randomIndex = rand() % countryCount;

        snprintf(cell, cellSize, "%s",
                 countries[randomIndex]);

    } else if (column == 5) {
        int randomIndex = rand() % NUM_PHONE_INDEXES;
        int phoneLine = rand() % PHONE_LINE_RANGE;

        snprintf(cell, cellSize, "%03d-%04d",
                 phoneIndexes[randomIndex], phoneLine);

    } else if (column == 6) {
        int randomSuffix = rand() % emailSuffixCount;

        snprintf(cell, cellSize, "%c%s@%s",
                 firstNames[firstNameIndex][0],
                 lastNames[lastNameIndex],
                 emailSuffixes[randomSuffix]);

    } else if (column == 7) {
        char sinNumber[SIN_LENGTH + 1];

        sinNumber[0] = '1' + rand() % 9;

        for (int i = 1; i < SIN_LENGTH; i++) {
            sinNumber[i] = '0' + rand() % 10;
        }

        sinNumber[SIN_LENGTH] = '\0';

        snprintf(cell, cellSize, "%s", sinNumber);

    } else if (column == 8) {
        char password[MAX_PASSWORD_LENGTH + 1];
        int passwordLength = MIN_PASSWORD_LENGTH +
            rand() % (MAX_PASSWORD_LENGTH - MIN_PASSWORD_LENGTH + 1);

        for (int i = 0; i < passwordLength; i++) {
            int randomIndex =
                rand() % (sizeof(allowedChars) - 1);

            password[i] = allowedChars[randomIndex];
        }

        password[passwordLength] = '\0';

        snprintf(cell, cellSize, "%s", password);
    }
}

// Generate each row in memory
// Returns 1 if OK, 0 on error (the caller frees the rows)
static int buildRows(GeneratedRow *rows, int rowCount,
                     int cols[], int numCols) {

    for (int row = 1; row <= rowCount; row++) {

        int firstNameIndex = 0;
        int lastNameIndex = 0;

        char rowBuffer[ROW_SIZE] = "";
        char cell[CELL_SIZE];
        size_t used = 0;

        if (fNameCount > 0) {
            firstNameIndex = rand() % fNameCount;
        }

        if (lNameCount > 0) {
            lastNameIndex = rand() % lNameCount;
        }

        for (int col = 0; col < numCols; col++) {

            cell[0] = '\0';

            makeCell(cell, sizeof(cell), cols[col], row,
                     firstNameIndex, lastNameIndex);

            // Remember the first selected column's value
            if (col == 0) {
                rows[row - 1].sortKey = malloc(strlen(cell) + 1);

                if (rows[row - 1].sortKey == NULL) {
                    printf("Could not allocate sorting key.\n");
                    return 0;
                }
                
                strcpy(rows[row - 1].sortKey, cell);
            }

            // Add a comma between CSV values
            if (col > 0) {
                rowBuffer[used++] = ',';
                rowBuffer[used] = '\0';
            }

            // add the current cell to the row
            size_t cellLength = strlen(cell);

            if (used + cellLength >= sizeof(rowBuffer)) {
                printf("Generated row is too long.\n");
                return 0;
            }

            memcpy(rowBuffer + used, cell, cellLength + 1);
            used += cellLength;
        }

        // Save the complete row
        rows[row - 1].csvLine = malloc(strlen(rowBuffer) + 1);

        if (rows[row - 1].csvLine == NULL) {
            printf("Could not allocate memory for a row.\n");
            return 0;
        }

        strcpy(rows[row - 1].csvLine, rowBuffer);
    }

    return 1;
}

void generate(int rowCount, char fileName[],
              int cols[], int numCols) {

    char fullFileName[FULL_FILE_NAME_SIZE];

    GeneratedRow *rows = NULL;

    // Build output filename
    snprintf(fullFileName, sizeof(fullFileName),
             "%s.csv", fileName);

    // Load only the files needed by the selected columns
    if (!loadFiles(cols, numCols)) {
        return;
    }

    // Allocate memory for all generated rows
    rows = calloc((size_t)rowCount, sizeof(GeneratedRow));

    // Generate each row in memory
    if (!buildRows(rows, rowCount, cols, numCols)) {
        freeRows(rows, rowCount);
        return;
    }

    // Sort by the first selected column
    sortRows(rows, rowCount, cols[0]);

    // Open the output CSV and write the header and rows
    if (!writeCSV(fullFileName, rows, rowCount, cols, numCols)) {
        freeRows(rows, rowCount);
        return;
    }

    // Release allocated memory
    freeRows(rows, rowCount);

    printf("Table written successfull to %s\n",
           fullFileName);
}