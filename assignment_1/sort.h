#ifndef SORT_H
#define SORT_H

typedef struct {
    char *sortKey; // The value of the column to sort by (first column)
    char *csvLine; // The complete CSV line for the row
} GeneratedRow;

void sortRows(GeneratedRow rows[], int rowCount, int sortColumn);

#endif