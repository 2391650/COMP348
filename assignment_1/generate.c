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
    FILE *emailSuffixesFile;
    
    char fullFileName[68];
    char firstNames[1000][64];
    char lastNames[1000][64];
    char countries[195][64];
    int phoneIndexes[] = {398, 270, 925, 867, 209, 429, 908, 997, 444, 219};
    char emailSuffixes[100][64];
    char allowedChars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!\"#$%&'()*+-./:;<=>?@[\\]^_`{|}~";    
    char line[64];

    int fNameCount = 0;
    int lNameCount = 0;
    int countryCount = 0;
    int emailSuffixCount = 0;

    // Check that the selected columns are implemented
    for (int i = 0; i < numCols; i++) {
        if (cols[i] != 1 && cols[i] != 2 && cols[i] != 3 && cols[i] != 4 && cols[i] != 5 && cols[i] != 6 && cols[i] != 7 && cols[i] != 8) {
            printf("Only columns 1, 2, 3, 4, 5, 6, 7 and 8 are implemented for now.\n");
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

        if (cols[i] == 6) {

            emailSuffixesFile = fopen("email_suffixes.txt", "r");

            if (emailSuffixesFile == NULL) {
                printf("Error opening email_suffixes.txt\n");
                fclose(file_ptr);
                return;
            }

            while (emailSuffixCount < 100 &&
                fgets(line, sizeof(line), emailSuffixesFile) != NULL) {

                line[strcspn(line, "\r\n")] = '\0';

                if (line[0] != '\0') {
                    snprintf(emailSuffixes[emailSuffixCount],
                            sizeof(emailSuffixes[emailSuffixCount]),
                            "%s", line);

                    emailSuffixCount++;
                }
            }

            fclose(emailSuffixesFile);

            if (emailSuffixCount == 0) {
                printf("No email suffixes found in email_suffixes.txt\n");
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
        } else if (cols[i] == 5) {
            fprintf(file_ptr, "Phone Number");
        } else if (cols[i] == 6) {
            fprintf(file_ptr, "Email Address");
        } else if (cols[i] == 7) {
            fprintf(file_ptr, "SIN");
        } else if (cols[i] == 8) {
            fprintf(file_ptr, "Password");
        }
    }

    fprintf(file_ptr, "\n");

    // Generate each row
    for (int row = 1; row <= rowCount; row++) {
        int firstNameIndex = 0;
        int lastNameIndex = 0;

        if (fNameCount > 0) {
            firstNameIndex = rand() % fNameCount;
        }

        if (lNameCount > 0) {
            lastNameIndex = rand() % lNameCount;
        }

        for (int col = 0; col < numCols; col++) {

            if (col > 0) {
                fprintf(file_ptr, ",");
            }

            if (cols[col] == 1) {
                fprintf(file_ptr, "%d", row);
            } else if (cols[col] == 2) {

                fprintf(file_ptr, "%s",
                        firstNames[firstNameIndex]);
            } else if (cols[col] == 3) {

                fprintf(file_ptr, "%s",
                        lastNames[lastNameIndex]);
            } else if (cols[col] == 4) {
                int randomIndex = rand() % countryCount;

                fprintf(file_ptr, "%s",
                        countries[randomIndex]);
            } else if (cols[col] == 5) {
                int randomIndex = rand() % 10;
                int phoneLine = rand() % 10000;

                fprintf(file_ptr, "%03d%04d",
                        phoneIndexes[randomIndex], phoneLine);
            } else if (cols[col] == 6) {
                int randomSuffix = rand() % emailSuffixCount;

                fprintf(file_ptr, "%c%s@%s",
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

                fprintf(file_ptr, "%s", sinNumber);
            } else if (cols[col] == 8) {
                char password[17];

                int passwordLength = 6 + rand() % 11;

                for (int i = 0; i < passwordLength; i++) {
                    int randomIndex = rand() % (sizeof(allowedChars) - 1);

                    password[i] = allowedChars[randomIndex];
                }

                password[passwordLength] = '\0';

                fprintf(file_ptr, "%s", password);
            }
        }

        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);

    printf("CSV file generated successfully: %s\n", fullFileName);
}