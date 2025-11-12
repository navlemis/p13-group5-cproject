#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "studentFunctions.h"
#include "parser.h"

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

void query_record(const char *args, Student *head)
{
    Student temp;
    if (!parse_fields(args, &temp) || temp.id == -1)
    {
        printf("CMS: Invalid QUERY format. Required: ID. Example: QUERY ID=123\n");
        return;
    }

    while (head)
    {
        if (head->id == temp.id)
        {
            printf("CMS: Record found.\n");
            printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");
            printf("%-8d\t%-20s\t%-30s\t%.1f\n", head->id, head->name, head->programme, head->mark);
            return;
        }

        head = head->next;
    }

    printf("CMS: Record with ID=%d not found.\n", temp.id);
}

void show_all_records(Student *head)
{
    if (!head)
    {
        printf("No records loaded.\n");
        return;
    }

    printf("Here are all the records found in the table \"StudentRecords\".\n");
    printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");

    while (head)
    {
        printf("%-8d\t%-20s\t%-30s\t%.1f\n", head->id, head->name, head->programme, head->mark);
        head = head->next;
    }
}

void update_record(const char *args, Student *head)
{
    Student temp;
    if (!parse_fields(args, &temp) || temp.id == -1)
    {
        printf("CMS: Invalid UPDATE format. Required: ID. Example: UPDATE ID=123\n");
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
        printf("CMS: The record with ID=%d does not exist.\n", temp.id);
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

    printf("CMS: The record with ID=%d is successfully updated.\n", temp.id);
    printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");
    printf("%-8d\t%-20s\t%-30s\t%.1f\n", current->id, current->name, current->programme, current->mark);
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

    printf("CMS: Summary of Student Records:\n");
    printf("Total number of students: %d\n", countStudents);
    printf("Average mark: %.1f\n", averageMark);
    printf("Highest mark: %.1f (Student: %s)\n", highest, highestMark_name);
    printf("Lowest mark: %.1f (Student: %s)\n", lowest, lowestMark_name);
}
