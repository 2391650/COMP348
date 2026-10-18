#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

void generate(int rowCount, char fileName[],
              int cols[], int numCols) {

    FILE *file_ptr;
    FILE *fNamesFile;
    FILE *lNamesFile;
    FILE *countriesFile;
    
    char fullFileName[68];
    char firstNames[1000][64];
    char lastNames[1000][64];
    char countries[195][64];
    char line[64];

    int fNameCount = 0;
    int lNameCount = 0;
    int countryCount = 0;

    // Check that the selected columns are implemented
    for (int i = 0; i < numCols; i++) {
        if (cols[i] != 1 && cols[i] != 2 && cols[i] != 3 && cols[i] != 4) {
            printf("Only columns 1, 2, 3, and 4 are implemented for now.\n");
            return;
        }
    }

    // Build the output filename
    snprintf(fullFileName, sizeof(fullFileName),
             "%s.csv", fileName);

    // Open the output CSV
    file_ptr = fopen(fullFileName, "w");

    if (file_ptr == NULL) {
        printf("Error opening output file!\n");
        return;
    }

    // Load from files
    for (int i = 0; i < numCols; i++) {
        if (cols[i] == 2) {

            fNamesFile = fopen("first_names.txt", "r");

            if (fNamesFile == NULL) {
                printf("Error opening first_names.txt\n");
                fclose(file_ptr);
                return;
            }

            while (fNameCount < 1000 &&
                   fgets(line, sizeof(line), fNamesFile) != NULL) {

                // Remove newline characters
                line[strcspn(line, "\r\n")] = '\0';

                // Skip empty lines
                if (line[0] != '\0') {
                    snprintf(firstNames[fNameCount],
                             sizeof(firstNames[fNameCount]),
                             "%s", line);

                    fNameCount++;
                }
            }

            fclose(fNamesFile);

            if (fNameCount == 0) {
                printf("No names found in first_names.txt\n");
                fclose(file_ptr);
                return;
            }
        }

        if (cols[i] == 3) {

            lNamesFile = fopen("last_names.txt", "r");

            if (lNamesFile == NULL) {
                printf("Error opening last_names.txt\n");
                fclose(file_ptr);
                return;
            }

            while (lNameCount < 1000 &&
                   fgets(line, sizeof(line), lNamesFile) != NULL) {

                // Remove newline characters
                line[strcspn(line, "\r\n")] = '\0';

                // Skip empty lines
                if (line[0] != '\0') {
                    snprintf(lastNames[lNameCount],
                             sizeof(lastNames[lNameCount]),
                             "%s", line);

                    lNameCount++;
                }
            }

            fclose(lNamesFile);

            if (lNameCount == 0) {
                printf("No names found in last_names.txt\n");
                fclose(file_ptr);
                return;
            }
        }

        if (cols[i] == 4) {
            
            countriesFile = fopen("countries.txt", "r");

            if (countriesFile == NULL) {
                printf("Error opening countries.txt\n");
                fclose(file_ptr);
                return;
            }

            while (countryCount < 195 &&
                   fgets(line, sizeof(line), countriesFile) != NULL) {

                // Remove newline characters
                line[strcspn(line, "\r\n")] = '\0';

                // Skip empty lines
                if (line[0] != '\0') {
                    snprintf(countries[countryCount],
                             sizeof(countries[countryCount]),
                             "%s", line);

                    countryCount++;
                }
            }

            fclose(countriesFile);

            if (countryCount == 0) {
                printf("No countries found in countries.txt\n");
                fclose(file_ptr);
                return;
            }
        }
    }

    // Write column headers in the selected order
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
        }
    }

    fprintf(file_ptr, "\n");

    // Generate each row
    for (int row = 1; row <= rowCount; row++) {

        for (int col = 0; col < numCols; col++) {

            if (col > 0) {
                fprintf(file_ptr, ",");
            }

            if (cols[col] == 1) {
                fprintf(file_ptr, "%d", row);
            } else if (cols[col] == 2) {
                int randomIndex = rand() % fNameCount;

                fprintf(file_ptr, "%s",
                        firstNames[randomIndex]);
            } else if (cols[col] == 3) {
                int randomIndex = rand() % lNameCount;

                fprintf(file_ptr, "%s",
                        lastNames[randomIndex]);
            } else if (cols[col] == 4) {
                int randomIndex = rand() % countryCount;

                fprintf(file_ptr, "%s",
                        countries[randomIndex]);
            }
        }

        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);

    printf("CSV file generated successfully: %s\n",
           fullFileName);
}