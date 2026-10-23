#ifndef SORT_H
#define SORT_H

typedef struct {
    char *sortKey; // First column
    char *csvLine; // Whole Row
} GeneratedRow;

void sortRows(GeneratedRow rows[], int rowCount, int sortColumn);

#endif