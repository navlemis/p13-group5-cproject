#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "loadSave.h"

int load_records(const char *filename, Student **head) {
    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    char line[256];
    int inTable = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "Table Name:", 11) == 0) {
            inTable = 1;
            continue;
        }

        if (inTable && strlen(line) > 0 && isdigit(line[0])) {
            Student *newNode = malloc(sizeof(Student));
            sscanf(line, "%d %49[^\t] %49[^\t] %f", &newNode->id, newNode->name, newNode->programme, &newNode->mark);
            newNode->next = NULL;

            if (*head == NULL) {
                *head = newNode;
            } else {
                Student *current = *head;
                while (current->next) current = current->next;
                current->next = newNode;
            }
        }
    }

    fclose(file);
    return 1;
}

int save_records(const char *filename, Student *head) {
    FILE *file = fopen(filename, "w");
    if (!file) return 0;

    fprintf(file, "Table Name: StudentRecords\n");

    while (head) {
        fprintf(file, "%d\t%s\t%s\t%.1f\n", head->id, head->name, head->programme, head->mark);
        head = head->next;
    }

    fclose(file);
    return 1;
}