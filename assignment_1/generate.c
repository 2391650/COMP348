#include <stdio.h>
#include "generate.h"

void generate(int rowCount, char fileName[]) {
    FILE *file_ptr;
    char fullFileName[68];

    snprintf(fullFileName, sizeof(fullFileName), "%s.csv", fileName);

    file_ptr = fopen(fullFileName, "w");

    if (file_ptr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file_ptr, "User ID\n"); // column header

    for (int i = 1; i <= rowCount; i++) {
        fprintf(file_ptr, "%d\n", i);
    }

    fclose(file_ptr);

    printf("CSV file generated successfully: %s\n", fullFileName);
}