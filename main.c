#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_INPUT_LENGTH 100
#define DB_NAME "Sample-CMS.txt"

//function to convert string to uppercase
void to_upper(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper((unsigned char) str[i]);
    }
}

//open command for first time booting up the program
int handle_open()
{
    FILE *file = fopen(DB_NAME, "r+");

    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    printf("The database file \"%s\" is successfully opened.\n", DB_NAME);
    fclose(file);
    return 1;
}

//quit to exit program
void handle_quit()
{
    printf("Exiting the CMS Database System. Goodbye!\n");
    exit(0);
}

//error handling for unknown command
void handle_unknown(const char *command)
{
    printf("Unknown command. Please enter a valid command.\n");
}

//main menu display options
void display_menu()
{
    printf("-------------------------------\n");
    printf("CMS Database System Commands:\n");
    printf("1. SHOW ALL - Display all contents in the database table.\n");
    printf("2. QUIT - Exit the program.\n");
    printf("Please enter a command: ");

}

//1. SHOW ALL function to load and display all records from the database table
void load_table(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int inTable = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "Table Name:", 11) == 0) {
            printf("Here are all the records found in the table \"%s\".\n", line + 12);
            inTable = 1;
            continue;
        }

        if (inTable && strlen(line) > 0) {
            printf("%s\n", line);
        }
    }

    fclose(file);
}

int main()
{
    char userCommand[MAX_INPUT_LENGTH];

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
            if (handle_open() == 1)
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
                    to_upper(subCommand);

                    if (strcmp(subCommand, "SHOW ALL") == 0)
                    {
                        load_table(DB_NAME);
                    }
                    else if (strcmp(subCommand, "QUIT") == 0)
                    {
                        handle_quit();
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

}
