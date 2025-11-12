#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studentFunctions.h"
#include "parser.h"
#include "utilities.h"

void insert_record(const char *args, Student **head) 
{
    Student temp; //temp student that holds parsed data
    if (!parse_fields(args, &temp))
    {
        //failed parsing returns error message, unlikely to reach here
        printf("CMS: Failed to parse INSERT fields.\n");
        return;
    }

    if (temp.id == -1 || strlen(temp.name) == 0 || strlen(temp.programme) == 0 || temp.mark < 0)
    {
        //failed INSERT command format due to missing fields
        printf("CMS: Invalid INSERT format. Required: ID, Name, Programme, Mark.\n");
        return;
    }

    Student *current = *head; //sets current student to the start of the linkedlist

    while (current) //loops while current is not null
    {
        if (current->id == temp.id)
        {
            //uses current student to check for duplicate ID
            printf("CMS: Record with ID=%d already exists.\n", temp.id);
            return;
        }
        current = current->next; //moves to next student in linkedlist, eventually stopps at null which comes after the end of last item in the linkedlist
    }

    Student *newNode = malloc(sizeof(Student));
    *newNode = temp;
    newNode->next = NULL;

    if (*head == NULL)
    {
        *head = newNode;
    }

    else
    {
        current = *head;
        while (current->next) current = current->next;
        current->next = newNode;
    }

    printf("CMS: Record with ID=%d successfully inserted.\n", temp.id);
}

void show_all_records(Student *head)
{
    if (!head)
    {
        printf("No records loaded.\n");
        return;
    }
    printf("\n");
    printf("================================================================================\n");
    printf("Here are all the records found in the table \"StudentRecords\".\n");
    printf("================================================================================\n");
    printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");
    printf("--------------------------------------------------------------------------------\n");

    while (head)
    {
        printf("%-8d\t%-20s\t%-30s\t%.1f\n", head->id, head->name, head->programme, head->mark);
        head = head->next;
    }
    printf("================================================================================\n");
}

void query_record(const char *args, Student *head) {
    if (!args || strlen(args) == 0) {
        printf("CMS: Invalid QUERY format. Examples:\n");
        printf("  QUERY ID=123\n");
        printf("  QUERY Name=Joshua\n");
        printf("  QUERY Programme=Software Engineering\n");
        return;
    }

    // Parse the fields from the arguments
    Student temp;
    parse_fields(args, &temp);

    // Determine what you are searching by; ID, Name, Programme (special features for Query)
    int searchByID = (temp.id != -1);
    int searchByName = (strlen(temp.name) > 0);
    int searchByProgramme = (strlen(temp.programme) > 0);

    // Special features for Query: prompt an example if command is invalid
    if (!searchByID && !searchByName && !searchByProgramme) {
        printf("CMS: No valid search field provided. Examples:\n");
        printf("  QUERY ID=123\n");
        printf("  QUERY Name=Joshua Chen\n");
        printf("  QUERY Programme=Software Engineering\n");
        return;
    }

    // matching records
    Student *current = head;
    int foundCount = 0;

    printf("\n========================================\n");
    printf("Search Results:\n");
    printf("========================================\n");

    while (current != NULL) {
        int matches = 0;

        // Check if current record matches the search criterias 
        if (searchByID && current->id == temp.id) {
            matches = 1;
        }
        if (searchByName && strlen(temp.name) > 0) {
            char currentNameUpper[88], searchNameUpper[88];
            strcpy(currentNameUpper, current->name);
            strcpy(searchNameUpper, temp.name);
            to_upper(currentNameUpper);
            to_upper(searchNameUpper);
            
            if (strstr(currentNameUpper, searchNameUpper) != NULL) {
                matches = 1;
            }
        }
        if (searchByProgramme && strlen(temp.programme) > 0) {
            char currentProgUpper[88], searchProgUpper[88];
            strcpy(currentProgUpper, current->programme);
            strcpy(searchProgUpper, temp.programme);
            to_upper(currentProgUpper);
            to_upper(searchProgUpper);
            
            if (strstr(currentProgUpper, searchProgUpper) != NULL) {
                matches = 1;
            }
        }

        if (matches) {
            if (foundCount > 0) {
                printf("----------------------------------------\n");
            }
            printf("ID          : %d\n", current->id);
            printf("Name        : %s\n", current->name);
            printf("Programme   : %s\n", current->programme);
            printf("Mark        : %.1f\n", current->mark);
            foundCount++;
        }

        current = current->next;
    }

    printf("========================================\n");
    
    if (foundCount == 0) {
        printf("CMS: No records found matching.\n");
    } else {
        printf("Total records found: %d\n", foundCount);
    }
}

void update_record(const char *args, Student *head)
{
    Student temp;
    if (!parse_fields(args, &temp) || temp.id == -1)
    {
        printf("CMS: Invalid UPDATE format. Example: UPDATE ID=123 Programme=NewProgramme OR Mark=85.5\n");
        return;
    }

    Student *current = head;
    while (current)
    {
        if (current->id == temp.id)
        {
            break;
        }
        current = current->next;
    }    

    if (!current)
    {
        printf("\n");
        printf("CMS: The record with ID=%d does not exist.\n", temp.id);
        return;
    }

    if (strlen(temp.programme) == 0 && temp.mark < 0)
    {
        printf("\n");
        printf("========================================\n");
        printf("CMS: Record with ID=%d exists but no fields provided to update. (Programme or Mark)\n", temp.id);
        printf("----------------------------------------\n");
        printf("ID          : %d\n", current->id);
        printf("Name        : %s\n", current->name);
        printf("Programme   : %s\n", current->programme);
        printf("Mark        : %.1f\n", current->mark);
        printf("========================================\n");
        printf("Example: UPDATE ID=%d Programme=NewProgramme OR Mark=85.5\n", temp.id);
        return;
    }

    if (strlen(temp.programme) > 0)
    {
        strcpy(current->programme, temp.programme);
    }

    if (temp.mark >= 0)
    {
        current->mark = temp.mark;
    }

    printf("\n");
    printf("========================================\n");
    printf("CMS: The record with ID=%d is successfully updated.\n", temp.id);
    printf("----------------------------------------\n");
    printf("ID          : %d\n", current->id);
    printf("Name        : %s\n", current->name);
    printf("Programme   : %s\n", current->programme);
    printf("Mark        : %.1f\n", current->mark);
    printf("========================================\n");
}

void show_summary(Student *head)
{
    if (!head)
    {
        printf("CMS: No records available to summarise.\n");
        return;
    }

    int countStudents = 0;
    float totalMarks = 0.0;
    float averageMark = 0.0;
    float highest = head->mark;
    float lowest = head->mark;
    char highestMark_name[88];
    char lowestMark_name[88];
    strcpy(highestMark_name, head->name);
    strcpy(lowestMark_name, head->name); 

    while (head)
    {
        countStudents++;
        totalMarks += head->mark;

        if (head->mark > highest)
        {
            highest = head->mark;
            strcpy(highestMark_name, head->name);
        }

        if (head->mark < lowest)
        {
            lowest = head->mark;
            strcpy(lowestMark_name, head->name);
        }
        head = head->next;
    }

    averageMark = totalMarks / countStudents;

    printf("\n");
    printf("========================================\n");
    printf("CMS: Summary of Student Records:\n");
    printf("----------------------------------------\n");
    printf("Total number of students: %d\n", countStudents);
    printf("Average mark: %.1f\n", averageMark);
    printf("Highest mark: %.1f (Student: %s)\n", highest, highestMark_name);
    printf("Lowest mark: %.1f (Student: %s)\n", lowest, lowestMark_name);
    printf("========================================\n");

}
