#include <stdio.h>

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
    printf("TableGen Menu\n");
    printf("-------------\n");
    printf("1. Generate new table\n");
    printf("2. Exit\n");
    printf("\nSelection: ");
}

void generateTable() {
    printf("Generating a new table...\n");
    // table logic
}