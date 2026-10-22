#include <stdio.h>
#include <string.h>
#include "io.h"

int loadStrings(const char *fileName,
                char strings[][64],
                int maxEntries) {
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        printf("Error opening %s\n", fileName);
        return -1;
    }

    char line[64];
    int count = 0;

    while (count < maxEntries &&
           fgets(line, sizeof(line), file) != NULL) {

        // Remove newline characters
        line[strcspn(line, "\r\n")] = '\0';

        // Skip empty lines
        if (line[0] != '\0') {
            snprintf(strings[count], 64, "%s", line);
            count++;
        }
    }

    fclose(file);
    return count;
}


// Write column headers in the selected order
static void writeHeader(FILE *file_ptr, int cols[], int numCols) {
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
}

// Open the output CSV, write the header and the sorted rows
int writeCSV(const char *fullFileName, GeneratedRow *rows,
             int rowCount, int cols[], int numCols) {

    FILE *file_ptr = fopen(fullFileName, "w");

    if (file_ptr == NULL) {
        printf("Error opening output file!\n");
        return 0;
    }
    // Write the header
    writeHeader(file_ptr, cols, numCols);

    // Writes the sorted rows
    for (int i = 0; i < rowCount; i++) {
        fprintf(file_ptr, "%s\n", rows[i].csvLine);
    }

    fclose(file_ptr);
    return 1;
}