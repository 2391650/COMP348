#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

void generate(int rowCount, char fileName[],
              int cols[], int numCols) {

    FILE *file_ptr;
    FILE *fNamesFile;

    char fullFileName[68];
    char firstNames[1000][64];
    char line[64];

    int nameCount = 0;

    // Check that the selected columns are implemented
    for (int i = 0; i < numCols; i++) {
        if (cols[i] != 1 && cols[i] != 2) {
            printf("Only columns 1 and 2 are implemented for now.\n");
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

    // Load first names only if column 2 was selected
    for (int i = 0; i < numCols; i++) {
        if (cols[i] == 2) {

            fNamesFile = fopen("first_names.txt", "r");

            if (fNamesFile == NULL) {
                printf("Error opening first_names.txt\n");
                fclose(file_ptr);
                return;
            }

            while (nameCount < 1000 &&
                   fgets(line, sizeof(line), fNamesFile) != NULL) {

                // Remove newline characters
                line[strcspn(line, "\r\n")] = '\0';

                // Skip empty lines
                if (line[0] != '\0') {
                    snprintf(firstNames[nameCount],
                             sizeof(firstNames[nameCount]),
                             "%s", line);

                    nameCount++;
                }
            }

            fclose(fNamesFile);

            if (nameCount == 0) {
                printf("No names found in first_names.txt\n");
                fclose(file_ptr);
                return;
            }

            break;
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
                int randomIndex = rand() % nameCount;

                fprintf(file_ptr, "%s",
                        firstNames[randomIndex]);
            }
        }

        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);

    printf("CSV file generated successfully: %s\n",
           fullFileName);
}