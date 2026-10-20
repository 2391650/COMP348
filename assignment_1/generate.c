#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"
#include "io.h"
#include "sort.h"

void generate(int rowCount, char fileName[],
              int cols[], int numCols) {

    FILE *file_ptr = NULL;

    char fullFileName[68];
    char firstNames[1000][64];
    char lastNames[1000][64];
    char countries[195][64];
    char emailSuffixes[100][64];

    int phoneIndexes[] = {
        398, 270, 925, 867, 209,
        429, 908, 997, 444, 219
    };

    char allowedChars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!\"#$%&'()*+-./:;<=>?@[\\]^_`{|}~";

    int fNameCount = 0;
    int lNameCount = 0;
    int countryCount = 0;
    int emailSuffixCount = 0;

    GeneratedRow *rows = NULL;

    // Check the selected columns.
    if (rowCount < 1 || numCols < 1) {
        printf("Invalid row count or column count.\n");
        return;
    }

    for (int i = 0; i < numCols; i++) {
        if (cols[i] < 1 || cols[i] > 8) {
            printf("Only columns 1 through 8 are implemented.\n");
            return;
        }
    }

    // Build the output filename.
    snprintf(fullFileName, sizeof(fullFileName),
             "%s.csv", fileName);

    // Load only the files needed by the selected columns.
    for (int i = 0; i < numCols; i++) {

        if (cols[i] == 2 && fNameCount == 0) {
            fNameCount = loadStrings(
                "first_names.txt", firstNames, 1000);
        }

        if (cols[i] == 3 && lNameCount == 0) {
            lNameCount = loadStrings(
                "last_names.txt", lastNames, 1000);
        }

        if (cols[i] == 4 && countryCount == 0) {
            countryCount = loadStrings(
                "countries.txt", countries, 195);
        }

        if (cols[i] == 6 && emailSuffixCount == 0) {
            emailSuffixCount = loadStrings(
                "email_suffixes.txt", emailSuffixes, 100);

            // otherwise email would be empty
            fNameCount = loadStrings(
                "first_names.txt", firstNames, 1000);
            
            lNameCount = loadStrings(
                "last_names.txt", lastNames, 1000);
        }
    }

    // Allocate memory for all generated rows.
    rows = calloc((size_t)rowCount, sizeof(GeneratedRow));

    if (rows == NULL) {
        printf("Could not allocate memory for rows.\n");
        return;
    }

    // Generate each row in memory.
    for (int row = 1; row <= rowCount; row++) {

        int firstNameIndex = 0;
        int lastNameIndex = 0;

        char rowBuffer[2048] = "";
        char cell[256];
        size_t used = 0;

        if (fNameCount > 0) {
            firstNameIndex = rand() % fNameCount;
        }

        if (lNameCount > 0) {
            lastNameIndex = rand() % lNameCount;
        }

        for (int col = 0; col < numCols; col++) {

            cell[0] = '\0';

            if (cols[col] == 1) {
                snprintf(cell, sizeof(cell), "%d", row);

            } else if (cols[col] == 2) {
                snprintf(cell, sizeof(cell), "%s",
                         firstNames[firstNameIndex]);

            } else if (cols[col] == 3) {
                snprintf(cell, sizeof(cell), "%s",
                         lastNames[lastNameIndex]);

            } else if (cols[col] == 4) {
                int randomIndex = rand() % countryCount;

                snprintf(cell, sizeof(cell), "%s",
                         countries[randomIndex]);

            } else if (cols[col] == 5) {
                int randomIndex = rand() % 10;
                int phoneLine = rand() % 10000;

                snprintf(cell, sizeof(cell), "%03d-%04d",
                         phoneIndexes[randomIndex], phoneLine);

            } else if (cols[col] == 6) {
                int randomSuffix = rand() % emailSuffixCount;

                snprintf(cell, sizeof(cell), "%c%s@%s",
                         firstNames[firstNameIndex][0],
                         lastNames[lastNameIndex],
                         emailSuffixes[randomSuffix]);

            } else if (cols[col] == 7) {
                char sinNumber[10];

                sinNumber[0] = '1' + rand() % 9;

                for (int i = 1; i < 9; i++) {
                    sinNumber[i] = '0' + rand() % 10;
                }

                sinNumber[9] = '\0';

                snprintf(cell, sizeof(cell), "%s", sinNumber);

            } else if (cols[col] == 8) {
                char password[17];
                int passwordLength = 6 + rand() % 11;

                for (int i = 0; i < passwordLength; i++) {
                    int randomIndex =
                        rand() % (sizeof(allowedChars) - 1);

                    password[i] = allowedChars[randomIndex];
                }

                password[passwordLength] = '\0';

                snprintf(cell, sizeof(cell), "%s", password);
            }

            // Remember the first selected column's value.
            if (col == 0) {
                rows[row - 1].sortKey = malloc(strlen(cell) + 1);

                if (rows[row - 1].sortKey == NULL) {
                    printf("Could not allocate sorting key.\n");

                    for (int i = 0; i < row; i++) {
                        free(rows[i].sortKey);
                        free(rows[i].csvLine);
                    }

                    free(rows);
                    return;
                }

                strcpy(rows[row - 1].sortKey, cell);
            }

            // Add a comma between CSV values.
            if (col > 0) {
                if (used + 1 >= sizeof(rowBuffer)) {
                    printf("Generated row is too long.\n");

                    for (int i = 0; i < row; i++) {
                        free(rows[i].sortKey);
                        free(rows[i].csvLine);
                    }

                    free(rows);
                    return;
                }

                rowBuffer[used++] = ',';
                rowBuffer[used] = '\0';
            }

            // Add the current cell to the row.
            size_t cellLength = strlen(cell);

            if (used + cellLength >= sizeof(rowBuffer)) {
                printf("Generated row is too long.\n");

                for (int i = 0; i < row; i++) {
                    free(rows[i].sortKey);
                    free(rows[i].csvLine);
                }

                free(rows);
                return;
            }

            memcpy(rowBuffer + used, cell, cellLength + 1);
            used += cellLength;
        }

        // Save the complete row.
        rows[row - 1].csvLine = malloc(strlen(rowBuffer) + 1);

        if (rows[row - 1].csvLine == NULL) {
            printf("Could not allocate memory for a row.\n");

            for (int i = 0; i < row; i++) {
                free(rows[i].sortKey);
                free(rows[i].csvLine);
            }

            free(rows);
            return;
        }

        strcpy(rows[row - 1].csvLine, rowBuffer);
    }

    // Sort by the first selected column.
    sortRows(rows, rowCount, cols[0]);

    // Open the output CSV.
    file_ptr = fopen(fullFileName, "w");

    if (file_ptr == NULL) {
        printf("Error opening output file!\n");

        for (int i = 0; i < rowCount; i++) {
            free(rows[i].sortKey);
            free(rows[i].csvLine);
        }

        free(rows);
        return;
    }

    // Write column headers in the selected order.
    for (int i = 0; i < numCols; i++) {

        if (i > 0) {
            fprintf(file_ptr, ",");
        }

        if (cols[i] == 1) {
            fprintf(file_ptr, "User ID");
        } else if (cols[i] == 2) {
            fprintf(file_ptr, "First Name");
        } else if (cols[i] == 3) {
            fprintf(file_ptr, "Last Name");
        } else if (cols[i] == 4) {
            fprintf(file_ptr, "Country");
        } else if (cols[i] == 5) {
            fprintf(file_ptr, "Phone");
        } else if (cols[i] == 6) {
            fprintf(file_ptr, "Email");
        } else if (cols[i] == 7) {
            fprintf(file_ptr, "SIN");
        } else if (cols[i] == 8) {
            fprintf(file_ptr, "Password");
        }
    }

    fprintf(file_ptr, "\n");

    // Write the sorted rows.
    for (int i = 0; i < rowCount; i++) {
        fprintf(file_ptr, "%s\n", rows[i].csvLine);
    }

    fclose(file_ptr);

    // Release allocated memory.
    for (int i = 0; i < rowCount; i++) {
        free(rows[i].sortKey);
        free(rows[i].csvLine);
    }

    free(rows);

    printf("CSV file generated successfully: %s\n",
           fullFileName);
}