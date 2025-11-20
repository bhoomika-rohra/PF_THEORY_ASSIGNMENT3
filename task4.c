#include <stdio.h>
#include <string.h>

void addBook(int capacity, int bookIDs[], int popularity[], int x, int y);
int accessBook(int capacity, int bookIDs[], int popularity[], int x);

int main() {
    int capacity, Q;
    printf("Enter shelf capacity and number of operations: ");
    scanf("%d %d", &capacity, &Q);

    int bookIDs[capacity];
    int popularity[capacity];

    for (int i = 0; i < capacity; i++) {
        bookIDs[i] = -1;  
        popularity[i] = 0;
    }

    for (int i = 0; i < Q; i++) {
        char command[10];
        int x, y;

        scanf("%s", command);

        if (strcmp(command, "ADD") == 0) {
            scanf("%d %d", &x, &y);
            addBook(capacity, bookIDs, popularity, x, y);
        } else if (strcmp(command, "ACCESS") == 0) {
            scanf("%d", &x);
            int score = accessBook(capacity, bookIDs, popularity, x);
            printf("%d\n", score);
        }
    }

    return 0;
}

void addBook(int capacity, int bookIDs[], int popularity[], int x, int y) {
    for (int i = 0; i < capacity; i++) {
        if (bookIDs[i] == x) {
            popularity[i] = y; 
            return;
        }
    }

    for (int i = 0; i < capacity; i++) {
        if (bookIDs[i] == -1) {
            bookIDs[i] = x;
            popularity[i] = y;
            return;
        }
    }

    bookIDs[0] = x;
    popularity[0] = y;
}

int accessBook(int capacity, int bookIDs[], int popularity[], int x) {
    for (int i = 0; i < capacity; i++) {
        if (bookIDs[i] == x){ 
            return popularity[i];
        }    
    }
    return -1;
}
