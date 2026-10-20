#ifndef SORT_H
#define SORT_H

typedef struct {
    char *sortKey;
    char *csvLine;
} GeneratedRow;

void sortRows(GeneratedRow rows[], int rowCount, int sortColumn);

#endif