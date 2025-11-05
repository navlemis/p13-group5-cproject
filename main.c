#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_INPUT_LENGTH 100
#define MAX_RECORDS 100
#define DB_NAME "Sample-CMS.txt"

//student "object" structure
typedef struct
{
    int id;
    char name[50];
    char programme[50];
    float mark;
} Student;

//global variable for everyone to access
Student records[MAX_RECORDS];
int recordCount = 0;
int isModified = 0;

//convert string to upper
void to_upper(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper((unsigned char) str[i]);
    }
}

//load records from file into Student array
int load_records(const char *filename, Student *records, int maxRecords) {
    FILE *file = fopen(filename, "r"); //open file as read mode

    char line[256];
    int count = 0;
    int inTable = 0;

    while (fgets(line, sizeof(line), file) && count < maxRecords) 
    {
        line[strcspn(line, "\n")] = 0; //strips newline \n character

        if (strncmp(line, "Table Name:", 11) == 0) 
        //if first 11 characters match "Table Name:", to detect start of table in .txt file
        {

            inTable = 1; // equals 1 to indicate table section started
            continue;
        }

        if (inTable && strlen(line) > 0 && isdigit(line[0])) 
        //intable must be 1, line after must not be empty, and check if first character is digit
        {
            sscanf(line, "%d %88[^\t] %88[^\t] %f", //parses each line into student record fields 
                   &records[count].id,
                   records[count].name,
                   records[count].programme,
                   &records[count].mark);
            count++;
        }
    }

    fclose(file);
    return count; //returns count to measure number of records loaded
}

// Handle OPEN command
int handle_open(Student *records, int maxRecords, int *recordCount)
{
    *recordCount = load_records(DB_NAME, records, maxRecords);
    printf("The database file \"%s\" is successfully opened.\n", DB_NAME);
    return 1;
}

// Handle QUIT command
void handle_quit()
{
    printf("Exiting the CMS Database System. Goodbye!\n");
    exit(0);
}

// Display declaration
void display_declaration()
{
    printf("\nDeclaration\n\n");
    printf("SIT's policy on copying does not allow students to copy code or assessments\n");
    printf("from others or AI. Students must ensure all work is original.\n\n");
    printf("We hereby declare that:\n");
    printf("- We understand and agree to the plagiarism policy.\n");
    printf("- We did not copy code from others or AI sources.\n");
    printf("- We did not share or upload our code for public access.\n");
    printf("- We agree to receive zero marks if plagiarism is detected.\n\n");
    printf("Declared by: P13 Group 5\n");
    printf("Team members:\n");
    printf("1. [Constance Cheah (Xie Kangyue)]\n");
    printf("2. [Ang Khim Loong Zareth]\n");
    printf("3. [Angelline Tan An Qi]\n");
    printf("4. [Elvan Sim Wei Han]\n");
    printf("5. [Low Dai Xuan]\n");
    printf("Date: November 26, 2025\n\n");
    printf("-------------------------------\n");
}

// Handle unknown commands
void handle_unknown(const char *command)
{
    printf("Unknown command. Please enter a valid command.\n");
}

// Display submenu options
void display_menu()
{
    printf("\n-------------------------------\n");
    printf("CMS Database System Commands:\n");
    printf("1. SHOW ALL - Display all contents in the database table.\n");
    printf("2. QUIT - Exit the program.\n");
    printf("3. INSERT ID=(id) Name=(Name) Programme=(programme) Mark=(mark) - Insert a new record into the database.\n");
    printf("4. QUERY ID=(id) - Search for a student record by ID.\n");
    printf("10. SAVE - Save changes to the database file after making edits.\n");
    printf("Please enter a command (Case Sensitive): ");
}

// Display all loaded records
void show_all_records(Student *records, int count)
{
    if (count == 0)
    {
        printf("No records loaded.\n");
        return;
    }

    printf("Here are all the records found in the table \"StudentRecords\".\n");
    printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-8d\t%-20s\t%-30s\t%.1f\n",
               records[i].id,
               records[i].name,
               records[i].programme,
               records[i].mark);
    }
}

//parser to extract fields
int parse_fields(const char *input, Student *s)
{
    if (!s) return 0;

    s->id = -1;
    s->mark = -1;
    s->name[0] = '\0';
    s->programme[0] = '\0';

    char buffer[256];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    // Find field positions
    char *idPtr = strstr(buffer, "ID=");
    char *namePtr = strstr(buffer, "Name=");
    char *progPtr = strstr(buffer, "Programme=");
    char *markPtr = strstr(buffer, "Mark=");

    // Parse ID
    if (idPtr) sscanf(idPtr, "ID=%d", &s->id);

    // Parse Mark
    if (markPtr) sscanf(markPtr, "Mark=%f", &s->mark);

    // Extract Name
    if (namePtr)
    {
        char *end = progPtr ? progPtr : markPtr ? markPtr : buffer + strlen(buffer);
        int len = end - (namePtr + 5);
        if (len > 0 && len < sizeof(s->name))
        {
            strncpy(s->name, namePtr + 5, len);
            s->name[len] = '\0';
        }
    }

    // Extract Programme
    if (progPtr)
    {
        char *end = markPtr ? markPtr : buffer + strlen(buffer);
        int len = end - (progPtr + 10);
        if (len > 0 && len < sizeof(s->programme))
        {
            strncpy(s->programme, progPtr + 10, len);
            s->programme[len] = '\0';
        }
    }

    return 1;
}
// Zareth's Part

void query_record(const char *args, Student *records, int count)
{
    Student temp;
    parse_fields(args, &temp);

    // Check if ID was provided
    if (temp.id == -1)
    {
        printf("CMS: Invalid QUERY format. Required: ID. Example: QUERY ID=123\n");
        return;
    }

    // Search for the record
    for (int i = 0; i < count; i++)
    {
        if (records[i].id == temp.id)
        {
            printf("CMS: The record with ID=%d is found in the data table.\n", temp.id);
            printf("ID\t\tName\t\t\tProgramme\t\t\tMark\n");
            printf("%-8d\t%-20s\t%-30s\t%.1f\n", 
                   records[i].id,
                   records[i].name,
                   records[i].programme,
                   records[i].mark);
            return;
        }
    }

    printf("CMS: The record with ID=%d does not exist.\n", temp.id);
}

// End of Zareth's Part

void insert_record(const char *args, Student *records, int *count, int maxRecords)
{
    if (*count >= maxRecords)
    {
        printf("CMS: Cannot insert. Maximum record limit reached.\n");
        return;
    }

    Student temp;
    parse_fields(args, &temp);

    // Validate required fields
    if (temp.id == -1 || strlen(temp.name) == 0 || strlen(temp.programme) == 0 || temp.mark < 0)
    {
        printf("CMS: Invalid INSERT format. Required: ID, Name, Programme, Mark.\n");
        return;
    }

    // Check for duplicate ID
    for (int i = 0; i < *count; i++)
    {
        if (records[i].id == temp.id)
        {
            printf("CMS: Record with ID=%d already exists.\n", temp.id);
            return;
        }
    }

    // Store the record
    records[*count] = temp;
    (*count)++;
    isModified = 1;

    printf("CMS: Record with ID=%d successfully inserted.\n", temp.id);
}

int save_records(const char *filename, Student *records, int count)
{
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error saving file");
        return 0;
    }

    fprintf(file, "Table Name: StudentRecords\n");

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\t%s\t%s\t%.1f\n",
                records[i].id,
                records[i].name,
                records[i].programme,
                records[i].mark);
    }

    fclose(file);
    return 1;
}

int main()
{
    char userCommand[MAX_INPUT_LENGTH];

    display_declaration();
    printf("Welcome to the CMS Database System.\n");

    while (1)
    {
        printf("\nEnter command (OPEN or QUIT): ");
        if (fgets(userCommand, sizeof(userCommand), stdin) == NULL)
        {
            printf("Input error.\n");
            continue;
        }

        userCommand[strcspn(userCommand, "\n")] = 0;
        to_upper(userCommand);

        if (strcmp(userCommand, "OPEN") == 0)
        {
            if (handle_open(records, MAX_RECORDS, &recordCount))
            {
                char subCommand[MAX_INPUT_LENGTH];
                while (1)
                {
                    display_menu();
                    if (fgets(subCommand, sizeof(subCommand), stdin) == NULL)
                    {
                        printf("Input error.\n");
                        continue;
                    }

                    subCommand[strcspn(subCommand, "\n")] = 0;
                    //to_upper(subCommand); COMMENTED FOR NOW, unsure if must be case sensitive

                    if (strcmp(subCommand, "SHOW ALL") == 0)
                    {
                        show_all_records(records, recordCount);
                    }
                    else if (strcmp(subCommand, "QUIT") == 0)
                    {
                        handle_quit();
                    }
                    else if (strncmp(subCommand, "INSERT", 6) == 0)
                    {
                    if (strlen(subCommand) > 7)
                        {
                            const char *insertArgs = subCommand + 7; // skip "INSERT "
                            insert_record(insertArgs, records, &recordCount, MAX_RECORDS);
                        }
                        else
                        {
                            printf("CMS: Invalid INSERT command format. Example: INSERT ID=123 Name=Elvan Programme=CMS Mark=88\n");
                        }
                    }
                    else if (strncmp(subCommand, "QUERY", 5) == 0)
                    {
                        if (strlen(subCommand) > 6)
                        {
                            const char *queryArgs = subCommand + 6; // skip "QUERY "
                            query_record(queryArgs, records, recordCount);
                        }
                        else
                        {
                            printf("CMS: Invalid QUERY format. Example: QUERY ID=123\n");
                        }
                    }
                    else if (strncmp(subCommand, "PARSE TEST", 10) == 0)
                    {
                        if (strlen(subCommand) > 11)
                        {
                            const char *parseArgs = subCommand + 11; // skip "PARSE TEST "
                            Student temp;
                            parse_fields(parseArgs, &temp);

                            printf("Parsed Fields:\n");
                            printf("ID: %d\n", temp.id);
                            printf("Name: %s\n", temp.name);
                            printf("Programme: %s\n", temp.programme);
                            printf("Mark: %.2f\n", temp.mark);
                        }
                        else 
                        {
                            printf("CMS: Invalid PARSE TEST format. Example: PARSE TEST ID=123 Name=Elvan Programme=CMS Mark=88\n");
                        }
                    }
                    else if (strncmp(subCommand, "SAVE", 4) == 0)
                    {
                        if (save_records(DB_NAME, records, recordCount))
                        {
                            isModified = 0;
                            printf("CMS: Changes saved to \"%s\" successfully.\n", DB_NAME);
                        }
                        else
                        {
                            printf("CMS: Failed to save changes to \"%s\".\n", DB_NAME);
                        }
                    }
                    else 
                    {
                        handle_unknown(subCommand);
                    }
                }
            }
        } 
        else if (strcmp(userCommand, "QUIT") == 0) 
        {
            handle_quit();
        } 
        else 
        {
            handle_unknown(userCommand);
        }
    }

    return 0;

}