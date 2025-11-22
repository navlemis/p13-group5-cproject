#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "studentFunctions.h"
#include "parser.h"
#include "utilities.h"
#include <stdlib.h>
#include <time.h>

LogEntry *logHead = NULL; //for logging

void free_records(Student *head)
{
    Student *temp;
    while (head)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void backup_records(Student *head, Student **backupHead)
{
    if (!backupHead) return;

    free_records(*backupHead);
    *backupHead = NULL;

    Student *src = head;
    Student *prevNew = NULL;

    while (src)
    {
        Student *newNode = malloc(sizeof(Student));
        if (!newNode)
        {
            printf("CMS: Unable to create backup (memory error).\n");
            free_records(*backupHead);
            *backupHead = NULL;
            return;
        }

        newNode->id = src->id;
        strcpy(newNode->name, src->name);
        strcpy(newNode->programme, src->programme);
        newNode->mark = src->mark;
        newNode->next = NULL;

        if (prevNew == NULL)
        {
            *backupHead = newNode;     
        }
        else
        {
            prevNew->next = newNode;    
        }

        prevNew = newNode;
        src = src->next;
    }
}

int undo_last_change(Student **head, Student **backupHead)
{
    if (!head || !backupHead || !*backupHead)
    {
        printf("CMS: No change to undo.\n");
        return 0;
    }

    free_records(*head);

    *head = *backupHead;
    *backupHead = NULL;

    printf("CMS: Last change has been undone.\n");
    return 1;
}

void insert_record(const char *args, Student **head) 
{
    Student temp; //temp student that holds parsed data
    if (!parse_fields(args, &temp)) //parses the fields from args into temp student
    {
        /*
        printf("Parsed ID=%d\n", temp.id);
        printf("Parsed Mark=%.2f\n", temp.mark);
        printf("Parsed Name=%s\n", temp.name);
        printf("Parsed Programme=%s\n", temp.programme);
        */

        //failed parsing returns error message
        printf("Failed to parse INSERT fields. Check your formatting please!\n");
        return;
    }


    if (temp.id == -1 || strlen(temp.name) == 0 || strlen(temp.programme) == 0 || temp.mark < 0)
    {
        //failed INSERT command format due to missing fields
        printf("Invalid INSERT format. Required: ID, Name, Programme, Mark.\n");
        return;
    }

    Student *current = *head; //sets current student to the start of the linkedlist

    while (current) //loops while current is not null
    {
        if (current->id == temp.id)
        {
            //uses current student to check for duplicate ID
            printf("Record with ID=%d already exists.\n", temp.id);
            return;
        }
        current = current->next; //moves to next student in linkedlist, eventually stopps at null which comes after the end of last item in the linkedlist
    }

    Student *newNode = malloc(sizeof(Student)); //alloc memory for newnode
    *newNode = temp; //temp student data copied into new node
    newNode->next = NULL; //points to next pointer as null to indicate last item

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

    printf("Record with ID=%d successfully inserted.\n", temp.id);
}

void show_all_records(Student *head, char *tableName)
{
    if (!head)
    {
        printf("No records loaded.\n");
        return;
    }

    
    printf("\n");
    printf("================================================================================\n");
    printf("Here are all the records found in the table %s.\n", tableName);
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
// Query
void query_record(const char *args, Student *head) {
    if (!args || strlen(args) == 0) {
        printf("Invalid QUERY format. Examples:\n");
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
        printf("No valid search field provided. Examples:\n");
        printf("  QUERY ID=123\n");
        printf("  QUERY Name=Joshua Chen\n");
        printf("  QUERY Programme=Software Engineering\n");
        return;
    }

    // matching records
    Student *current = head;
    int foundCount = 0;

    printf("\n");
    printf("================================================================================\n");
    printf("Search Results:\n");
    printf("================================================================================\n");
    printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");
    printf("--------------------------------------------------------------------------------\n");

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
            printf("%-8d\t%-20s\t%-30s\t%.1f\n", current->id, current->name, current->programme, current->mark);
            foundCount++;
        }

        current = current->next;
    }

    printf("================================================================================\n");
    
    if (foundCount == 0) {
        printf("No records found matching.\n");
    } else {
        printf("Total records found: %d\n", foundCount);
    }
}
// end of Query

void update_record(const char *args, Student *head)
{
    Student temp;
    if (!parse_fields(args, &temp) || temp.id == -1)
    {
        printf("Invalid UPDATE format. Example: UPDATE ID=123 Programme=NewProgramme OR Mark=85.5\n");
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
        printf("The record with ID=%d does not exist.\n", temp.id);
        return;
    }

    if (strlen(temp.programme) == 0 && temp.mark < 0)
    {
        printf("\n");
        printf("================================================================================\n");
        printf("Record with ID=%d exists but no fields provided to update. (Programme or Mark)\n", temp.id);
        printf("================================================================================\n");
        printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("%-8d\t%-20s\t%-30s\t%.1f\n", current->id, current->name, current->programme, current->mark);
        printf("================================================================================\n");
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
    printf("================================================================================\n");
    printf("The record with ID=%d is successfully updated.\n", temp.id);
    printf("================================================================================\n");
    printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("%-8d\t%-20s\t%-30s\t%.1f\n", current->id, current->name, current->programme, current->mark);
    printf("================================================================================\n");
}

void show_summary(Student *head)
{
    if (!head)
    {
        printf("No records available to summarise.\n");
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
    printf("================================================================================\n");
    printf("Summary of Student Records:\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("%-30s: %d\n", "Total number of students", countStudents);
    printf("%-30s: %.1f\n", "Average mark", averageMark);
    printf("%-30s: %.1f (Student: %s)\n", "Highest mark", highest, highestMark_name);
    printf("%-30s: %.1f (Student: %s)\n", "Lowest mark", lowest, lowestMark_name);
    printf("================================================================================\n");

}

// Sorting Function
static int cmp_id_asc(const void *a, const void *b) {
    const Student *A = *(const Student * const *)a;
    const Student *B = *(const Student * const *)b;
    return (A->id > B->id) - (A->id < B->id);
}

static int cmp_id_desc(const void *a, const void *b) {
    return -cmp_id_asc(a,b);
}

static int cmp_mark_asc(const void *a, const void *b) {
    const Student *A = *(const Student * const *)a;
    const Student *B = *(const Student * const *)b;
    if (A->mark < B->mark) return -1;
    if (A->mark > B->mark) return 1;
    return 0;
}

static int cmp_mark_desc(const void *a, const void *b) {
    return -cmp_mark_asc(a,b);
}

void show_all_sorted(Student *head, const char *field, const char *order)
{
    if (!head) {
        printf("No records loaded.\n");
        return;
    }

    // count nodes
    int count = 0;
    Student *cur = head;
    while (cur) { count++; cur = cur->next; }

    // build array of pointers
    Student **arr = malloc(sizeof(Student*) * count);
    if (!arr) { printf("Memory allocation error.\n"); return; }
    cur = head; int idx = 0;
    while (cur) { arr[idx++] = cur; cur = cur->next; }

    // decide comparator by field (case-sensitive: ID or MARK expected) and order (ASC/DESC)
    const char *ord = (order && strlen(order) > 0) ? order : "ASC";

    if (field && strcmp(field, "ID") == 0) {
        if (strcmp(ord, "DESC") == 0) qsort(arr, count, sizeof(Student*), cmp_id_desc);
        else qsort(arr, count, sizeof(Student*), cmp_id_asc);
    } else if (field && strcmp(field, "MARK") == 0) {
        if (strcmp(ord, "DESC") == 0) qsort(arr, count, sizeof(Student*), cmp_mark_desc);
        else qsort(arr, count, sizeof(Student*), cmp_mark_asc);
    } else {
        // unknown, default to ID ascending
        qsort(arr, count, sizeof(Student*), cmp_id_asc);
    }

    // print header and rows
    printf("\n");
    printf("=====================================================\n");
    printf("Here are all the records found in the table \"Student Records\" (sorted by %s %s).\n", (field?field:"ID"), ord);
    printf("=====================================================\n");
    printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < count; ++i) {
        Student *s = arr[i];
        printf("%-8d\t%-20s\t%-30s\t%.1f\n", s->id, s->name, s->programme, s->mark);
    }
    printf("=====================================================\n");

    free(arr);
}

// Delete Function
static inline void trim_eol(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n && (s[n-1] == '\n' || s[n-1] == '\r')) s[--n] = '\0';
}


static int delete_record_by_id(Student **head, int id) {
    if (!head || !*head) return 0;
    Student **pp = head;
    while (*pp && (*pp)->id != id) {
        pp = &(*pp)->next;
    }
    if (!*pp) return 0;
    Student *victim = *pp;
    *pp = victim->next;
    free(victim);
    return 1;
}


int delete_command(const char *args, Student **head) {
    if (!args || !head) return -1;

    Student temp;
    temp.id = -1; temp.mark = -1.0f;
    temp.name[0] = '\0'; temp.programme[0] = '\0';

    
    if (!parse_fields(args, &temp) || temp.id == -1) {
        printf("Invalid DELETE format. Example: DELETE ID=2401234\n");
        return -1;
    }
    

    Student *cur = *head;
    while (cur && cur->id != temp.id) cur = cur->next;
    if (!cur) {
        printf("The record with ID=%d does not exist.\n", temp.id);
        return 0;
    }

    printf("Are you sure you want to delete record with ID=%d? Type \"Y\" to Confirm or type \"N\" to cancel.\n", temp.id);
    char yn[16];
    if (!fgets(yn, sizeof(yn), stdin)) {
        printf("The deletion is cancelled.\n");
        return 0;
    }
    trim_eol(yn);
    to_upper(yn);

    if (yn[0] != 'Y') {
        printf("The deletion is cancelled.\n");
        return 0;
    }

    if (delete_record_by_id(head, temp.id)) {
        printf("The record with ID=%d is successfully deleted.\n", temp.id);
        return 1;
    } else {
        printf("The record with ID=%d does not exist.\n", temp.id);
        return 0;
    }
}

void log_command(const char *command)
{
  
    LogEntry *newEntry = malloc(sizeof(LogEntry));
    if (!newEntry) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(newEntry->timestamp, sizeof(newEntry->timestamp), "%d-%m-%Y %H:%M:%S", t);
    snprintf(newEntry->command, sizeof(newEntry->command), "%s", command);

    newEntry->next = logHead;
    logHead = newEntry;
}

void export_log(const char *filename)
{
  
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to export log.\n");
        return;
    }

    LogEntry *curr = logHead;
    while (curr) {
        fprintf(fp, "[%s] %s\n", curr->timestamp, curr->command);
        curr = curr->next;
    }

    fclose(fp);
    printf("Session log exported to \"%s\".\n", filename);
}