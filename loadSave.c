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

/*
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
*/

int save_records(const char *filename, Student *head) {
    FILE *fp_in = fopen(filename, "r");
    FILE *fp_out = fopen("temp.txt", "w");
    if (!fp_in || !fp_out) return 0;

    char line[512];
    int foundTable = 0;
    int inTable = 0;
    

    while (fgets(line, sizeof(line), fp_in)) {
        // Detect start of table
        if (!foundTable && strncmp(line, "Table Name:", 11) == 0) {
            foundTable = 1;
            fputs(line, fp_out); // Write "Table Name:" line

            // Write new table header
            fprintf(fp_out, "ID\tName\tProgramme\tMark\n");

            // Write new student records
            Student *current = head;
            while (current) {
                fprintf(fp_out, "%d\t%s\t%s\t%.1f\n",
                        current->id, current->name, current->programme, current->mark);
                current = current->next;
            }

            // Skip old table lines until blank line or EOF
            while (fgets(line, sizeof(line), fp_in)) {
                if (line[0] == '\n') {
                    fputs(line, fp_out); // Write the blank line to preserve spacing
                    break;
                }
            }

            inTable = 1;
            continue;
        }

        // Copy everything before "Table Name:" or after the blank line
        if (!inTable) {
            fputs(line, fp_out);
        }
    }

    fclose(fp_in);
    fclose(fp_out);
    remove(filename);
    rename("temp.txt", filename);

    return foundTable;
}