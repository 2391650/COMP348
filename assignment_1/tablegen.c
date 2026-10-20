#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "generate.h"

void startPage();
void configureTable();

int main() {
    srand((unsigned int)time(NULL)); // Seed the random number generator

    int choice;

    while (1) {
        // system("clear"); // use with docker

        startPage();
        
        scanf("%d", &choice);
        
        if (choice == 1) {
            configureTable();
        } else if (choice == 2) {
            printf("Goodbye and thanks for using TableGen\n");
            break;
        } else {
            printf("Invalid selection. Please try again.\n");
        }
    }
    return 0;
}

void startPage() {
    printf("\nTableGen Menu\n");
    printf("-------------\n");
    printf("1. Generate new table\n");
    printf("2. Exit\n");
    printf("\nSelection: ");
}

void configureTable() {
    // system("clear"); // use with docker

    printf("\nColumn Options\n");
    printf("--------------\n");
    printf("1. User ID          5. Phone Number\n");
    printf("2. First name       6. Email address\n");
    printf("3. Last name        7. SIN\n");
    printf("4. Country          8. Password\n");
    printf("\nEnter column list (comma-separated, no spaces): ");
    
    char columns[16]; 
    int rowCount;
    char fileName[64];
    
    scanf("%15s", columns); // column string input
    printf("Enter row count (1 < n < 1M): ");
    scanf("%d", &rowCount);
    printf("Enter output file name (no suffix): ");
    scanf("%63s", fileName);

    int cols[8];
    int numCols = 0;

    // from column string to array of integers
    char *token = strtok(columns, ",");
    while (token != NULL) {
        cols[numCols++] = atoi(token);   // atoi: a(scii) to i(nt) 
        token = strtok(NULL, ",");       // NULL means "continue where you left off"
    }

    printf("\nSummary of properties:\n");
    printf(" Columns: ");
    for (int i = 0; i < numCols; i++) {
        printf("%d", cols[i]);
        if (i < numCols - 1) {
            printf(",");
        }
    }
    printf("\n");
    printf(" Row count: %d\n", rowCount);
    printf(" File name: %s\n", fileName);
    generate(rowCount, fileName, cols, numCols);

    // code from assignment pdf
    char proceed;
    printf("\nPress 'c' or 'C' to continue ");
    do{
        scanf("%c", &proceed);
    } while ( (proceed != 'c') && (proceed != 'C'));
}