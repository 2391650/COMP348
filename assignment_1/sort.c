#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

static int columnToSort;

// Compares two rows with quicksort
static int compareRows(const void *a, const void *b) {
    const GeneratedRow *rowA = (const GeneratedRow *)a;
    const GeneratedRow *rowB = (const GeneratedRow *)b;

    if (columnToSort == 1) {
        long numberA = strtol(rowA->sortKey, NULL, 10);
        long numberB = strtol(rowB->sortKey, NULL, 10);

        if (numberA < numberB) {
            return -1;
        }

        if (numberA > numberB) {
            return 1;
        }

        return 0;
    }

    return strcmp(rowA->sortKey, rowB->sortKey);
}

void sortRows(GeneratedRow rows[], int rowCount, int sortColumn) {
    if (rows == NULL || rowCount < 2) {
        return;
    }

    columnToSort = sortColumn;

    qsort(
        rows,
        rowCount,
        sizeof(GeneratedRow),
        compareRows
    );
}