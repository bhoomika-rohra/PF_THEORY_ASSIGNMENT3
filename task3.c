#include <stdio.h>
#include <string.h>

void displayEmployees(int n, int ids[100], char names[100][50], char designations[100][50], double salaries[100]);
void findHighestSalary(int n, int ids[100], char names[100][50], char designations[100][50], double salaries[100]);
void searchEmployee(int n, int ids[100], char names[100][50], char designations[100][50], double salaries[100]);
void giveBonus(int n, double salaries[100], double threshold);

int main() {
    int n;
    int ids[100];
    char names[100][50];
    char designations[100][50];
    double salaries[100];

    printf("Enter number of employees: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nEnter details for Employee %d\n", i + 1);
        printf("ID: ");
        scanf("%d", &ids[i]);
        printf("Name: ");
        scanf("%s", names[i]);
        printf("Designation: ");
        scanf("%s", designations[i]);
        printf("Salary: ");
        scanf("%lf", &salaries[i]);
    }

    displayEmployees(n, ids, names, designations, salaries);
    findHighestSalary(n, ids, names, designations, salaries);
    searchEmployee(n, ids, names, designations, salaries);
    giveBonus(n, salaries, 50000);
    printf("\nEmployee records after applying bonus (if any):\n");
    displayEmployees(n, ids, names, designations, salaries);

    return 0;
}

void displayEmployees(int n, int ids[100], char names[100][50], char designations[100][50], double salaries[100]) {
    printf("\nID\tName\t\tDesignation\tSalary\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%-10s\t%-10s\t%.2f\n", ids[i], names[i], designations[i], salaries[i]);
    }
}

void findHighestSalary(int n, int ids[100], char names[100][50], char designations[100][50], double salaries[100]) {
    int index = 0;
    for (int i = 1; i < n; i++) {
        if (salaries[i] > salaries[index])
            index = i;
    }
    printf("\nEmployee with Highest Salary:\n");
    printf("ID: %d\nName: %s\nDesignation: %s\nSalary: %.2f\n",
           ids[index], names[index], designations[index], salaries[index]);
}

void searchEmployee(int n, int ids[100], char names[100][50], char designations[100][50], double salaries[100]) {
    int choice;
    printf("\nSearch Employee By:\n1. ID\n2. Name\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        int searchId;
        printf("Enter Employee ID: ");
        scanf("%d", &searchId);
        for (int i = 0; i < n; i++) {
            if (ids[i] == searchId) {
                printf("Employee Found: %d %s %s %.2f\n", ids[i], names[i], designations[i], salaries[i]);
                return;
            }
        }
        printf("Employee with ID %d not found.\n", searchId);
    } else if (choice == 2) {
        char searchName[50];
        printf("Enter Employee Name: ");
        scanf("%s", searchName);
        for (int i = 0; i < n; i++) {
            if (strcmp(names[i], searchName) == 0) {
                printf("Employee Found: %d %s %s %.2f\n", ids[i], names[i], designations[i], salaries[i]);
                return;
            }
        }
        printf("Employee with Name %s not found.\n", searchName);
    } else {
        printf("Invalid choice.\n");
    }
}

void giveBonus(int n, double salaries[100], double threshold) {
    for (int i = 0; i < n; i++) {
        if (salaries[i] < threshold) {
            salaries[i] *= 1.10;
        }
    }
}
