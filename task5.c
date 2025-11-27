#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

typedef struct { 
    char **lines;
    int size;
    int capacity;
} Editor;

Editor initEditor() {
    Editor e;
    e.capacity = 2;
    e.size = 0;
    e.lines = (char **)malloc(e.capacity * sizeof(char *));
    if (!e.lines) error(1, 0, "Allocation failed for lines array");
    return e;
}

void ensureCapacity(Editor *e) {
    if (e->size >= e->capacity) {
        e->capacity *= 2;
        char **temp = realloc(e->lines, e->capacity * sizeof(char *));
        if (!temp) error(1, 0, "Realloc failed for lines array");
        e->lines = temp;
    }
}

void insertLine(Editor *e, int index, const char *text) {
    if (index < 0 || index > e->size) {
        printf("Invalid index\n");
        return;
    }
    ensureCapacity(e);
    for (int i = e->size; i > index; i--) {
        e->lines[i] = e->lines[i - 1];
    }
    int len = strlen(text);
    e->lines[index] = (char *)malloc(len + 1);
    if (!e->lines[index]) error(1, 0, "Line allocation failed");
    strcpy(e->lines[index], text);
    e->size++;
}

void deleteLine(Editor *e, int index) {
    if (index < 0 || index >= e->size) {
        printf("Invalid index.\n");
        return;
    }
    free(e->lines[index]);
    for (int i = index; i < e->size - 1; i++) {
        e->lines[i] = e->lines[i + 1];
    }
    e->size--;
}

void printAllLines(Editor *e){
    for (int i = 0; i < e->size; i++) {
        printf("%d: %s", i, e->lines[i]);
    }
}

void shrinkToFit(Editor *e){
    char **temp = realloc(e->lines, e->size * sizeof(char *));
    if (!temp && e->size > 0) error(1, 0, "Shrink failed");
    e->lines = temp;
    e->capacity = e->size;
}

void saveToFile(Editor e, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) error(1, 0, "Cannot open file for writing");
    for (int i = 0; i < e.size; i++) {
        fprintf(f, "%s", e.lines[i]);
    }
    fclose(f);
}

void loadFromFile(Editor *e, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) error(1, 0, "Cannot open file for reading");
    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), f)) {
        insertLine(e, e->size, buffer);
    }
    fclose(f);
}

void freeAll(Editor *e) {
    for (int i = 0; i < e->size; i++) {
        free(e->lines[i]);
    }
    free(e->lines);
}

int main() {
    Editor editor = initEditor();
    int choice, index;
    char text[1000];
    char filename[100];

    while (1) {
        printf("\n--- TEXT EDITOR ---\n");
        printf("1. Insert Line\n2. Delete Line\n3. Print All Lines\n4. Shrink To Fit\n5. Save To File\n6. Load From File\n7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            printf("Enter index to insert: ");
            scanf("%d", &index);
            getchar();
            printf("Enter text: ");
            fgets(text, sizeof(text), stdin);
            insertLine(&editor, index, text);
        } 
        else if (choice == 2) {
            printf("Enter index to delete: ");
            scanf("%d", &index);
            deleteLine(&editor, index);
        } 
        else if (choice == 3) {
            printAllLines(&editor);
        } 
        else if (choice == 4) {
            shrinkToFit(&editor);
            printf("Memory shrunk to fit lines\n");
        } 
        else if (choice == 5) {
            printf("Enter filename to save: ");
            scanf("%s", filename);
            saveToFile(editor, filename);
            printf("File saved successfully\n");
        } 
        else if (choice == 6) {
            printf("Enter filename to load: ");
            scanf("%s", filename);
            loadFromFile(&editor, filename);
            printf("File loaded successfully\n");
        } 
        else if (choice == 7) {
            freeAll(&editor);
            printf("Exiting editor...\n");
            break;
        } 
        else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
