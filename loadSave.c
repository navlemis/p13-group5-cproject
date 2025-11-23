#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "loadSave.h"

int load_records(const char *filename, Student **head, char *tableName)
{
    FILE *file = fopen(filename, "r"); //opens file in read mode


    if (!file)
    {
        //prints error if fail
        printf("Error opening file, are you sure the file exists?\n");
        return 0;
    }


    char line[256]; //variable to hold each line during while loop
    int inTable = 0; //marker to indicate if in table section

    while (fgets(line, sizeof(line), file)) //reads each line
    {
        line[strcspn(line, "\n")] = 0; //remove back newline character from fgets

        if (strncmp(line, "Table Name:", 11) == 0) //table name detection
        {
            sscanf(line, "Table Name: %127[^\n]", tableName); //stores in tableName variable 
            inTable = 1; 
            continue;
        }

        if (inTable && strlen(line) > 0 && isdigit(line[0])) //checks for table, line length, and if ID is present
        {
            Student *newNode = malloc(sizeof(Student)); //new node
            sscanf(line, "%d %49[^\t] %49[^\t] %f", &newNode->id, newNode->name, newNode->programme, &newNode->mark);
            newNode->next = NULL;

            if (*head == NULL)
            {
                *head = newNode; //first node
            }
            
            else
            {
                Student *current = *head;
                while (current->next) current = current->next;
                current->next = newNode; //append to end of linkedlist
            }
        }
    }

    fclose(file); //close

    if (!inTable)
    {
        printf("Table not found. Ensure you are opening the correct .txt file.\n"); //checks for table presence
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