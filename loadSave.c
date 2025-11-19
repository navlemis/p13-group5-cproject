#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "loadSave.h"

int load_records(const char *filename, Student **head, char *tableName)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file, are you sure the file exists?\n");
        return 0;
    }
    char line[256];
    int inTable = 0;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "Table Name:", 11) == 0)
        {
            sscanf(line, "Table Name: %127[^\n]", tableName);
            inTable = 1;
            continue;
        }

        if (inTable && strlen(line) > 0 && isdigit(line[0]))
        {
            Student *newNode = malloc(sizeof(Student));
            sscanf(line, "%d %49[^\t] %49[^\t] %f", &newNode->id, newNode->name, newNode->programme, &newNode->mark);
            newNode->next = NULL;

            if (*head == NULL)
            {
                *head = newNode;
            }
            
            else
            {
                Student *current = *head;
                while (current->next) current = current->next;
                current->next = newNode;
            }
        }
    }

    fclose(file);

    if (!inTable)
    {
        printf("Table not found. Ensure you are opening the correct .txt file.\n");
        return 0;
    }

    return 1;
}

int save_records(const char *filename, Student *head)
{
    FILE *fp_out = fopen("temp.txt", "w");
    if (!fp_out) return 0;

    // Write table name and header
    fprintf(fp_out, "Table Name: %s\n", filename); // Or use a stored tableName
    fprintf(fp_out, "ID\tName\tProgramme\tMark\n");

    // Write student records
    Student *current = head;
    while (current)
    {
        fprintf(fp_out, "%d\t%s\t%s\t%.1f\n",
                current->id, current->name, current->programme, current->mark);
        current = current->next;
    }

    fprintf(fp_out, "\n"); // Optional spacing

    fclose(fp_out);
    remove(filename);
    rename("temp.txt", filename);
    return 1;
}