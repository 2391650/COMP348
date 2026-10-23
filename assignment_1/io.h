#ifndef IO_H
#define IO_H

#include "sort.h"

int loadStrings(const char *fileName,
                char strings[][64],
                int maxEntries);

int writeCSV(const char *fullFileName, GeneratedRow *rows,
             int rowCount, int cols[], int numCols);

#endif