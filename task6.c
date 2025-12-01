#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int studentID;
    char fullName[100];
    char batch[32];
    char membershipType[16];
    char registrationDate[11];
    char dateOfBirth[11];
    char interestIn[16];
} Student;

static Student *db = NULL;
static size_t dbCount = 0;
static size_t dbCap = 0;
const char *DB_FILENAME = "members.dat";

int ensureCapacity(size_t needed) {
    if (needed <= dbCap) return 1;
    size_t newCap = dbCap ? dbCap * 2 : 8;
    while (newCap < needed) newCap *= 2;
    Student *tmp = realloc(db, newCap * sizeof(Student));
    if (!tmp) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }
    db = tmp;
    dbCap = newCap;
    return 1;
}

int loadDatabase(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        dbCount = 0;
        return 0;
    }
    Student tmp;
    dbCount = 0;
    while (fread(&tmp, sizeof(Student), 1, fp) == 1) {
        if (!ensureCapacity(dbCount + 1)) {
            fclose(fp);
            return 0;
        }
        db[dbCount++] = tmp;
    }
    fclose(fp);
    return 1;
}

int saveDatabase(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open file for writing: %s\n", filename);
        return 0;
    }
    if (dbCount > 0) {
        size_t written = fwrite(db, sizeof(Student), dbCount, fp);
        if (written != dbCount) {
            fprintf(stderr, "Failed to write all records to file\n");
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int findIndexByID(int studentID) {
    for (size_t i = 0; i < dbCount; ++i) {
        if (db[i].studentID == studentID) return (int)i;
    }
    return -1;
}

int addStudent(Student s, const char *filename) {
    if (findIndexByID(s.studentID) != -1) {
        fprintf(stderr, "Student ID %d already exists\n", s.studentID);
        return 0;
    }
    if (!ensureCapacity(dbCount + 1)) return 0;
    db[dbCount++] = s;
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        fprintf(stderr, "Failed to open file for appending: %s\n", filename);
        return 0;
    }
    if (fwrite(&s, sizeof(Student), 1, fp) != 1) {
        fprintf(stderr, "Failed to append record to file\n");
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

int updateStudent(int studentID) {
    int idx = findIndexByID(studentID);
    if (idx == -1) {
        fprintf(stderr, "Student ID %d not found\n", studentID);
        return 0;
    }
    printf("Updating student: %d - %s\n", db[idx].studentID, db[idx].fullName);
    printf("Current batch: %s\n", db[idx].batch);
    printf("Enter new batch (CS/SE/Cyber Security/AI) or press Enter to keep: ");
    char input[128];
    getchar();
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t ln = strlen(input);
        if (ln > 0 && input[ln-1] == '\n') input[ln-1] = '\0';
        if (strlen(input) > 0) {
            strncpy(db[idx].batch, input, sizeof(db[idx].batch)-1);
            db[idx].batch[sizeof(db[idx].batch)-1] = '\0';
        }
    }
    printf("Current membership type: %s\n", db[idx].membershipType);
    printf("Enter new membership type (IEEE/ACM) or press Enter to keep: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t ln = strlen(input);
        if (ln > 0 && input[ln-1] == '\n') input[ln-1] = '\0';
        if (strlen(input) > 0) {
            strncpy(db[idx].membershipType, input, sizeof(db[idx].membershipType)-1);
            db[idx].membershipType[sizeof(db[idx].membershipType)-1] = '\0';
        }
    }
    if (!saveDatabase(DB_FILENAME)) {
        fprintf(stderr, "Failed to save database after update\n");
        return 0;
    }
    printf("Update successful\n");
    return 1;
}

int deleteStudent(int studentID) {
    int idx = findIndexByID(studentID);
    if (idx == -1) {
        fprintf(stderr, "Student ID %d not found\n", studentID);
        return 0;
    }
    for (size_t i = idx; i + 1 < dbCount; ++i) {
        db[i] = db[i+1];
    }
    dbCount--;
    if (!saveDatabase(DB_FILENAME)) {
        fprintf(stderr, "Failed to save database after deletion\n");
        return 0;
    }
    printf("Student %d deleted\n", studentID);
    return 1;
}

void printStudent(const Student *s) {
    printf("ID: %d | Name: %s | Batch: %s | Membership: %s | RegDate: %s | DOB: %s | Interest: %s\n",
           s->studentID, s->fullName, s->batch, s->membershipType, s->registrationDate, s->dateOfBirth, s->interestIn);
}

void viewAll() {
    if (dbCount == 0) {
        printf("No records found\n");
        return;
    }
    for (size_t i = 0; i < dbCount; ++i) printStudent(&db[i]);
}

void generateBatchReport() {
    printf("Enter batch to report (CS/SE/Cyber Security/AI): ");
    char batchQuery[64];
    getchar();
    if (fgets(batchQuery, sizeof(batchQuery), stdin) == NULL) return;
    size_t ln = strlen(batchQuery);
    if (ln > 0 && batchQuery[ln-1] == '\n') batchQuery[ln-1] = '\0';
    printf("Enter membership filter (IEEE/ACM/Both): ");
    char memFilter[16];
    if (fgets(memFilter, sizeof(memFilter), stdin) == NULL) return;
    ln = strlen(memFilter);
    if (ln > 0 && memFilter[ln-1] == '\n') memFilter[ln-1] = '\0';
    int found = 0;
    for (size_t i = 0; i < dbCount; ++i) {
        int batchMatch = (strcasecmp(db[i].batch, batchQuery) == 0);
        int memMatch = 0;
        if (strcasecmp(memFilter, "Both") == 0) memMatch = 1;
        else memMatch = (strcasecmp(db[i].membershipType, memFilter) == 0);
        if (batchMatch && memMatch) {
            printStudent(&db[i]);
            found = 1;
        }
    }
    if (!found) printf("No records match the criteria\n");
}

int readInt(const char *prompt) {
    int v;
    printf("%s", prompt);
    while (scanf("%d", &v) != 1) {
        while (getchar() != '\n');
        printf("Invalid input. %s", prompt);
    }
    return v;
}

void readStringTrim(const char *prompt, char *buffer, size_t bufsize) {
    printf("%s", prompt);
    getchar();
    if (fgets(buffer, bufsize, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t ln = strlen(buffer);
    if (ln > 0 && buffer[ln-1] == '\n') buffer[ln-1] = '\0';
}

void registerNewStudent() {
    Student s;
    printf("Enter Student ID (integer): ");
    while (scanf("%d", &s.studentID) != 1) {
        while (getchar() != '\n');
        printf("Invalid. Enter Student ID: ");
    }
    if (findIndexByID(s.studentID) != -1) {
        printf("Student ID already exists. Aborting registration.\n");
        return;
    }
    getchar();
    printf("Enter full name: ");
    if (fgets(s.fullName, sizeof(s.fullName), stdin) == NULL) s.fullName[0] = '\0';
    size_t ln = strlen(s.fullName);
    if (ln > 0 && s.fullName[ln-1] == '\n') s.fullName[ln-1] = '\0';
    printf("Enter batch (CS/SE/Cyber Security/AI): ");
    if (fgets(s.batch, sizeof(s.batch), stdin) == NULL) s.batch[0] = '\0';
    ln = strlen(s.batch);
    if (ln > 0 && s.batch[ln-1] == '\n') s.batch[ln-1] = '\0';
    printf("Enter membership type (IEEE/ACM): ");
    if (fgets(s.membershipType, sizeof(s.membershipType), stdin) == NULL) s.membershipType[0] = '\0';
    ln = strlen(s.membershipType);
    if (ln > 0 && s.membershipType[ln-1] == '\n') s.membershipType[ln-1] = '\0';
    printf("Enter registration date (YYYY-MM-DD): ");
    if (fgets(s.registrationDate, sizeof(s.registrationDate), stdin) == NULL) s.registrationDate[0] = '\0';
    ln = strlen(s.registrationDate);
    if (ln > 0 && s.registrationDate[ln-1] == '\n') s.registrationDate[ln-1] = '\0';
    printf("Enter date of birth (YYYY-MM-DD): ");
    if (fgets(s.dateOfBirth, sizeof(s.dateOfBirth), stdin) == NULL) s.dateOfBirth[0] = '\0';
    ln = strlen(s.dateOfBirth);
    if (ln > 0 && s.dateOfBirth[ln-1] == '\n') s.dateOfBirth[ln-1] = '\0';
    printf("Enter interest (IEEE/ACM/Both): ");
    if (fgets(s.interestIn, sizeof(s.interestIn), stdin) == NULL) s.interestIn[0] = '\0';
    ln = strlen(s.interestIn);
    if (ln > 0 && s.interestIn[ln-1] == '\n') s.interestIn[ln-1] = '\0';
    if (addStudent(s, DB_FILENAME)) printf("Student registered successfully\n");
}

void stressTest() {
    const char *names[] = {"Ali Khan","Aisha Siddiqui","Bilal Ahmed","Fatima Noor","Hassan Ali","Mariam Khan","Saad Malik","Zainab Raza","Omar Farooq","Sana Iqbal"};
    const char *batches[] = {"CS","SE","Cyber Security","AI"};
    const char *members[] = {"IEEE","ACM"};
    const char *interests[] = {"IEEE","ACM","Both"};
    srand((unsigned)time(NULL));
    int initial = 0;
    for (int i = 0; i < 25; ++i) {
        Student s;
        s.studentID = 1000 + rand() % 9000;
        strncpy(s.fullName, names[rand() % (sizeof(names)/sizeof(names[0]))], sizeof(s.fullName)-1);
        s.fullName[sizeof(s.fullName)-1] = '\0';
        strncpy(s.batch, batches[rand() % 4], sizeof(s.batch)-1);
        s.batch[sizeof(s.batch)-1] = '\0';
        strncpy(s.membershipType, members[rand() % 2], sizeof(s.membershipType)-1);
        s.membershipType[sizeof(s.membershipType)-1] = '\0';
        snprintf(s.registrationDate, sizeof(s.registrationDate), "2025-%02d-%02d", 1 + rand()%12, 1 + rand()%28);
        snprintf(s.dateOfBirth, sizeof(s.dateOfBirth), "200%u-%02d-%02d", 0 + rand()%3, 1 + rand()%12, 1 + rand()%28);
        strncpy(s.interestIn, interests[rand() % 3], sizeof(s.interestIn)-1);
        s.interestIn[sizeof(s.interestIn)-1] = '\0';
        if (findIndexByID(s.studentID) == -1) {
            addStudent(s, DB_FILENAME);
            ++initial;
        }
        if (initial >= 30) break;
    }
    printf("Added %d random students\n", initial);
    int deletes = 0;
    for (int i = 0; i < 8 && dbCount > 0; ++i) {
        int idx = rand() % dbCount;
        int id = db[idx].studentID;
        if (deleteStudent(id)) ++deletes;
    }
    printf("Deleted %d random students\n", deletes);
    int updates = 0;
    for (int i = 0; i < 6 && dbCount > 0; ++i) {
        int idx = rand() % dbCount;
        int id = db[idx].studentID;
        strncpy(db[idx].batch, batches[rand() % 4], sizeof(db[idx].batch)-1);
        db[idx].batch[sizeof(db[idx].batch)-1] = '\0';
        strncpy(db[idx].membershipType, members[rand() % 2], sizeof(db[idx].membershipType)-1);
        db[idx].membershipType[sizeof(db[idx].membershipType)-1] = '\0';
        ++updates;
    }
    saveDatabase(DB_FILENAME);
    printf("Updated %d random records\n", updates);
}

void menuLoop() {
    int choice = 0;
    while (1) {
        printf("\n--- Membership Manager ---\n");
        printf("1. Register a new student\n");
        printf("2. Update membership or batch\n");
        printf("3. Delete a student\n");
        printf("4. View all registrations\n");
        printf("5. Generate batch-wise report\n");
        printf("6. Stress test (add/update/delete many records)\n");
        printf("7. Exit\n");
        choice = readInt("Choose an option: ");
        if (choice == 1) registerNewStudent();
        else if (choice == 2) {
            int id = readInt("Enter Student ID to update: ");
            updateStudent(id);
        } else if (choice == 3) {
            int id = readInt("Enter Student ID to delete: ");
            deleteStudent(id);
        } else if (choice == 4) viewAll();
        else if (choice == 5) generateBatchReport();
        else if (choice == 6) stressTest();
        else if (choice == 7) {
            if (!saveDatabase(DB_FILENAME)) fprintf(stderr, "Warning: failed to save database on exit\n");
            break;
        } else printf("Invalid choice\n");
    }
}

int main() {
    if (!loadDatabase(DB_FILENAME)) {
        printf("No existing database found or failed to open. Starting fresh.\n");
    } else {
        printf("Loaded %zu records from %s\n", dbCount, DB_FILENAME);
    }
    menuLoop();
    free(db);
    return 0;
}
