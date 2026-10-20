#ifndef GENERATE_H
#define GENERATE_H

/* Column numbers, as shown in the Column Options menu. */
#define COL_USER_ID     1
#define COL_FIRST_NAME  2
#define COL_LAST_NAME   3
#define COL_COUNTRY     4
#define COL_PHONE       5
#define COL_EMAIL       6
#define COL_SIN         7
#define COL_PASSWORD    8

void generate(int rowCount, char fileName[],
              int cols[], int numCols);

#endif
