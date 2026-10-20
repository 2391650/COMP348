#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"
#include "io.h"
#include "sort.h"

/* Sizes of the input files (from the assignment description). */
#define MAX_FIRST_NAMES    1000
#define MAX_LAST_NAMES     1000
#define MAX_COUNTRIES      195
#define MAX_EMAIL_SUFFIXES 100

/* Buffer sizes. */
#define CELL_SIZE 256     /* one value, e.g. a single name or password */
#define ROW_SIZE  2048    /* one complete CSV line                      */

#define NUM_PHONE_INDEXES 10
#define MIN_PASSWORD_LEN  6
#define MAX_PASSWORD_LEN  16

/* Everything we load from the text files, kept together in one struct. */
typedef struct {
    char firstNames[MAX_FIRST_NAMES][STR_LEN];
    char lastNames[MAX_LAST_NAMES][STR_LEN];
    char countries[MAX_COUNTRIES][STR_LEN];
    char emailSuffixes[MAX_EMAIL_SUFFIXES][STR_LEN];

    int fNameCount;
    int lNameCount;
    int countryCount;
    int emailSuffixCount;
} GenData;

static const int phoneIndexes[NUM_PHONE_INDEXES] = {
    398, 270, 925, 867, 209,
    429, 908, 997, 444, 219
};

/* All printable non-space characters EXCEPT the comma. */
static const char allowedChars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!\"#$%&'()*+-./:;<=>?@[\\]^_`{|}~";


/* ------------------------------------------------------------------ */
/* Generators for individual values                                    */
/* ------------------------------------------------------------------ */

/* Writes something like "908-1937" into out. */
static void generatePhone(char *out, size_t size) {
    int index = phoneIndexes[rand() % NUM_PHONE_INDEXES];
    int line = rand() % 10000;

    snprintf(out, size, "%03d-%04d", index, line);
}

/* Writes a 9-digit number into out (out must hold at least 10 chars). */
static void generateSIN(char *out) {
    out[0] = '1' + rand() % 9;          /* first digit is never 0 */

    for (int i = 1; i < 9; i++) {
        out[i] = '0' + rand() % 10;
    }

    out[9] = '\0';
}

/* Writes a 6-16 character password into out (out must hold 17 chars). */
static void generatePassword(char *out) {
    int length = MIN_PASSWORD_LEN +
                 rand() % (MAX_PASSWORD_LEN - MIN_PASSWORD_LEN + 1);

    for (int i = 0; i < length; i++) {
        out[i] = allowedChars[rand() % (sizeof(allowedChars) - 1)];
    }

    out[length] = '\0';
}

/* Produces the text for ONE cell of ONE row. */
static void generateCell(int column, int rowNumber, const GenData *data,
                         int firstIdx, int lastIdx,
                         char *cell, size_t size) {
    cell[0] = '\0';

    switch (column) {
    case COL_USER_ID:
        snprintf(cell, size, "%d", rowNumber);
        break;

    case COL_FIRST_NAME:
        snprintf(cell, size, "%s", data->firstNames[firstIdx]);
        break;

    case COL_LAST_NAME:
        snprintf(cell, size, "%s", data->lastNames[lastIdx]);
        break;

    case COL_COUNTRY:
        snprintf(cell, size, "%s",
                 data->countries[rand() % data->countryCount]);
        break;

    case COL_PHONE:
        generatePhone(cell, size);
        break;

    case COL_EMAIL:
        snprintf(cell, size, "%c%s@%s",
                 data->firstNames[firstIdx][0],
                 data->lastNames[lastIdx],
                 data->emailSuffixes[rand() % data->emailSuffixCount]);
        break;

    case COL_SIN:
        generateSIN(cell);
        break;

    case COL_PASSWORD:
        generatePassword(cell);
        break;
    }
}


/* ------------------------------------------------------------------ */
/* Helpers for building rows                                           */
/* ------------------------------------------------------------------ */

/* Returns a malloc'd copy of text (caller must free), or NULL. */
static char *copyString(const char *text) {
    char *copy = malloc(strlen(text) + 1);

    if (copy != NULL) {
        strcpy(copy, text);
    }

    return copy;
}

/* Adds cell (preceded by a comma if addComma) to the end of rowBuffer.
   Returns 0 on success, -1 if it would not fit. */
static int appendCell(char *rowBuffer, size_t bufferSize,
                      const char *cell, int addComma) {
    size_t used = strlen(rowBuffer);
    size_t needed = strlen(cell) + (addComma ? 1 : 0);

    if (used + needed + 1 > bufferSize) {
        return -1;
    }

    if (addComma) {
        strcat(rowBuffer, ",");
    }

    strcat(rowBuffer, cell);
    return 0;
}

/* Fills in one GeneratedRow: its sort key and its full CSV line.
   Returns 0 on success, -1 on failure. */
static int buildRow(GeneratedRow *row, int rowNumber,
                    const int cols[], int numCols, const GenData *data) {
    char rowBuffer[ROW_SIZE] = "";
    char cell[CELL_SIZE];
    int firstIdx = 0;
    int lastIdx = 0;

    /* Pick the name once per row so first name, last name and email agree. */
    if (data->fNameCount > 0) {
        firstIdx = rand() % data->fNameCount;
    }

    if (data->lNameCount > 0) {
        lastIdx = rand() % data->lNameCount;
    }

    for (int col = 0; col < numCols; col++) {
        generateCell(cols[col], rowNumber, data,
                     firstIdx, lastIdx, cell, sizeof(cell));

        /* The first selected column is the sort column. */
        if (col == 0) {
            row->sortKey = copyString(cell);

            if (row->sortKey == NULL) {
                return -1;
            }
        }

        if (appendCell(rowBuffer, sizeof(rowBuffer), cell, col > 0) != 0) {
            printf("Generated row is too long.\n");
            return -1;
        }
    }

    row->csvLine = copyString(rowBuffer);

    return (row->csvLine == NULL) ? -1 : 0;
}

/* Frees every string, then the array itself. Safe on rows that were
   never filled in, because calloc set their pointers to NULL. */
static void freeRows(GeneratedRow *rows, int rowCount) {
    for (int i = 0; i < rowCount; i++) {
        free(rows[i].sortKey);
        free(rows[i].csvLine);
    }

    free(rows);
}


/* ------------------------------------------------------------------ */
/* Setup helpers                                                       */
/* ------------------------------------------------------------------ */

/* Returns 1 if the request makes sense, 0 otherwise. */
static int validateRequest(const int cols[], int numCols, int rowCount) {
    if (rowCount < 1 || numCols < 1) {
        printf("Invalid row count or column count.\n");
        return 0;
    }

    for (int i = 0; i < numCols; i++) {
        if (cols[i] < COL_USER_ID || cols[i] > COL_PASSWORD) {
            printf("Only columns 1 through 8 are implemented.\n");
            return 0;
        }
    }

    return 1;
}

/* Loads one text file into list unless it is already loaded.
   Returns 0 on success, -1 on failure. */
static int loadList(const char *fileName, char list[][STR_LEN],
                    int maxEntries, int *count) {
    if (*count > 0) {
        return 0;
    }

    *count = loadStrings(fileName, list, maxEntries);

    if (*count <= 0) {
        printf("Could not load %s.\n", fileName);
        return -1;
    }

    return 0;
}

/* Loads only the files that the selected columns need.
   Returns 0 on success, -1 on failure. */
static int loadRequiredFiles(const int cols[], int numCols, GenData *data) {
    int needFirst = 0;
    int needLast = 0;
    int needCountry = 0;
    int needSuffix = 0;

    for (int i = 0; i < numCols; i++) {
        switch (cols[i]) {
        case COL_FIRST_NAME: needFirst = 1;                  break;
        case COL_LAST_NAME:  needLast = 1;                   break;
        case COL_COUNTRY:    needCountry = 1;                break;
        case COL_EMAIL:      needFirst = needLast = needSuffix = 1; break;
        }
    }

    if (needFirst &&
        loadList("first_names.txt", data->firstNames,
                 MAX_FIRST_NAMES, &data->fNameCount) != 0) {
        return -1;
    }

    if (needLast &&
        loadList("last_names.txt", data->lastNames,
                 MAX_LAST_NAMES, &data->lNameCount) != 0) {
        return -1;
    }

    if (needCountry &&
        loadList("countries.txt", data->countries,
                 MAX_COUNTRIES, &data->countryCount) != 0) {
        return -1;
    }

    if (needSuffix &&
        loadList("email_suffixes.txt", data->emailSuffixes,
                 MAX_EMAIL_SUFFIXES, &data->emailSuffixCount) != 0) {
        return -1;
    }

    return 0;
}


/* ------------------------------------------------------------------ */
/* The one public function                                             */
/* ------------------------------------------------------------------ */

void generate(int rowCount, char fileName[], int cols[], int numCols) {
    GenData data;
    GeneratedRow *rows;
    char fullFileName[68];

    if (!validateRequest(cols, numCols, rowCount)) {
        return;
    }

    memset(&data, 0, sizeof(data));
    snprintf(fullFileName, sizeof(fullFileName), "%s.csv", fileName);

    if (loadRequiredFiles(cols, numCols, &data) != 0) {
        return;
    }

    /* calloc zeroes the memory, so every pointer starts out NULL. */
    rows = calloc((size_t)rowCount, sizeof(GeneratedRow));

    if (rows == NULL) {
        printf("Could not allocate memory for rows.\n");
        return;
    }

    for (int row = 1; row <= rowCount; row++) {
        if (buildRow(&rows[row - 1], row, cols, numCols, &data) != 0) {
            printf("Could not build row %d.\n", row);
            freeRows(rows, rowCount);
            return;
        }
    }

    sortRows(rows, rowCount, cols[0]);

    if (writeTable(fullFileName, cols, numCols, rows, rowCount) != 0) {
        freeRows(rows, rowCount);
        return;
    }

    freeRows(rows, rowCount);

    printf("\nTable written successfully to %s\n", fullFileName);
}