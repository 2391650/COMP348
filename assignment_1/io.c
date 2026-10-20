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