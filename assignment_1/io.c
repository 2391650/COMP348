#include <stdio.h>
#include <string.h>
#include "io.h"
#include "generate.h"

int loadStrings(const char *fileName,
                char strings[][STR_LEN],
                int maxEntries) {
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        printf("Error opening %s\n", fileName);
        return -1;
    }

    char line[STR_LEN];
    int count = 0;

    while (count < maxEntries &&
           fgets(line, sizeof(line), file) != NULL) {

        /* Remove newline characters */
        line[strcspn(line, "\r\n")] = '\0';

        /* Skip empty lines */
        if (line[0] != '\0') {
            snprintf(strings[count], STR_LEN, "%s", line);
            count++;
        }
    }

    fclose(file);
    return count;
}

/* Returns the header text for a column number. */
static const char *columnName(int column) {
    switch (column) {
    case COL_USER_ID:    return "User ID";
    case COL_FIRST_NAME: return "First Name";
    case COL_LAST_NAME:  return "Last Name";
    case COL_COUNTRY:    return "Country";
    case COL_PHONE:      return "Phone Number";
    case COL_EMAIL:      return "Email Address";
    case COL_SIN:        return "SIN";
    case COL_PASSWORD:   return "Password";
    default:             return "";
    }
}

int writeTable(const char *path,
               const int cols[], int numCols,
               const GeneratedRow rows[], int rowCount) {
    FILE *file = fopen(path, "wt");

    if (file == NULL) {
        printf("Error opening output file!\n");
        return -1;
    }

    /* Header row, in the order the user chose. */
    for (int i = 0; i < numCols; i++) {
        if (i > 0) {
            fputs(",", file);
        }

        fputs(columnName(cols[i]), file);
    }

    fputs("\n", file);

    /* Data rows (already sorted). */
    for (int i = 0; i < rowCount; i++) {
        fputs(rows[i].csvLine, file);
        fputs("\n", file);
    }

    fclose(file);
    return 0;
}