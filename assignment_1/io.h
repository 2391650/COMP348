#ifndef IO_H
#define IO_H

#include "sort.h"

/* Maximum length (including the '\0') of one string loaded from a file. */
#define STR_LEN 64

/* Reads up to maxEntries non-empty lines from fileName into strings.
   Returns the number of lines read, or -1 if the file can't be opened. */
int loadStrings(const char *fileName,
                char strings[][STR_LEN],
                int maxEntries);

/* Writes the header row and all rows to path.
   Returns 0 on success, -1 if the file can't be opened. */
int writeTable(const char *path,
               const int cols[], int numCols,
               const GeneratedRow rows[], int rowCount);

#endif