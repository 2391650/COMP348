#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

void startPage();
void generateTable();

int main() {

    int choice;

    while (1) {
        
        startPage();
        
        scanf("%d", &choice);
        
        if (choice == 1) {
            generateTable();
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

void generateTable() {
    // system("clear"); // ill use it when i get docker installed

    printf("\nColumn Options\n");
    printf("--------------\n");
    printf("1. User ID          5. Phone Number\n");
    printf("2. First name       6. Email address\n");
    printf("3. Last name        7. SIN\n");
    printf("4. Country          8. Password\n");
    printf("\nEnter column list (comma-separated, no spaces): ");
    // ----------
    
    char columns[64]; 
    int rowCount;
    char fileName[64];
    
    scanf("%63s", columns);
    printf("Enter row count (1 < n < 1M): ");
    scanf("%d", &rowCount);
    printf("Enter output file name (no suffix): ");
    scanf("%63s", fileName);
    // --------------

    int cols[8];
    int numCols = 0;

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
    generate(rowCount, fileName);
}