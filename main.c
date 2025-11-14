#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "studentFunctions.h"
#include "parser.h"
#include "loadSave.h"
#include "utilities.h"

#define MAX_INPUT_LENGTH 100
#define DB_NAME "Sample-CMS.txt"

int main() {
    char userCommand[MAX_INPUT_LENGTH];
    Student *head = NULL;
    int isModified = 0;

    display_declaration();
    printf("Welcome to the CMS Database System.\n");

    while (1) {
        printf("\nEnter command (OPEN or QUIT): ");
        if (fgets(userCommand, sizeof(userCommand), stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }

    userCommand[strcspn(userCommand, "\n")] = 0;

        if (strcmp(userCommand, "OPEN") == 0) {
            if (load_records(DB_NAME, &head)) {
                printf("The database file \"%s\" is successfully opened.\n", DB_NAME);

                char subCommand[MAX_INPUT_LENGTH];
                while (1) {
                    display_menu();
                    if (fgets(subCommand, sizeof(subCommand), stdin) == NULL) {
                        printf("Input error.\n");
                        continue;
                    }

                    subCommand[strcspn(subCommand, "\n")] = 0;

                    if (strncmp(subCommand, "SHOW ALL", 8) == 0) {
                        /* support optional: SHOW ALL SORT BY <ID|MARK> (ascending only) */
                        char *sortPtr = strstr(subCommand, "SORT BY");
                        if (sortPtr) {
                            sortPtr += strlen("SORT BY");
                            while (*sortPtr == ' ') sortPtr++;
                            char field[16] = {0};
                            char order[16] = {0};
                            sscanf(sortPtr, "%15s %15s", field, order);
                            if (strlen(order) == 0) strcpy(order, "ASC");
                            if ((strcmp(field, "ID") == 0) || (strcmp(field, "MARK") == 0)) {
                                show_all_sorted(head, field, order);
                            } else {
                                printf("Invalid SORT field. Use ID or MARK (case-sensitive).\n");
                            }
                        } else {
                            show_all_records(head);
                        }
                    }
                    else if (strcmp(subCommand, "QUIT") == 0) {
                        printf("Exiting the CMS Database System. Goodbye!\n");
                        return 0;
                    }
                    else if (strncmp(subCommand, "INSERT", 6) == 0) {
                        if (strlen(subCommand) > 7) {
                            const char *insertArgs = subCommand + 7;
                            insert_record(insertArgs, &head);
                            isModified = 1;
                        } else {
                            printf("CMS: Invalid INSERT format. Example: INSERT ID=123 Name=Elvan Programme=CMS Mark=88\n");
                        }
                    }
                    else if (strncmp(subCommand, "QUERY", 5) == 0) {
                        if (strlen(subCommand) > 6) {
                            const char *queryArgs = subCommand + 6;
                            query_record(queryArgs, head);
                        } else {
                            printf("CMS: Invalid QUERY format. Example: QUERY ID=123\n");
                        }
                    }
                    else if (strncmp(subCommand, "UPDATE", 6) == 0) {
                        if (strlen(subCommand) > 7) {
                            const char *updateArgs = subCommand + 7;
                            update_record(updateArgs, head);
                            isModified = 1;
                        } else {
                            printf("CMS: Invalid UPDATE format. Example: UPDATE ID=123 Programme=NewProgramme Mark=85.5\n");
                        }
                    }
                    else if (strcmp(subCommand, "SHOW SUMMARY") == 0) {
                        show_summary(head);
                    } 
                    else if (strncmp(subCommand, "PARSE TEST", 10) == 0) {
                        if (strlen(subCommand) > 11) {
                            const char *parseArgs = subCommand + 11;
                            Student temp;
                            parse_fields(parseArgs, &temp);

                            printf("Parsed Fields:\n");
                            printf("ID: %d\n", temp.id);
                            printf("Name: %s\n", temp.name);
                            printf("Programme: %s\n", temp.programme);
                            printf("Mark: %.2f\n", temp.mark);
                        } else {
                            printf("CMS: Invalid PARSE TEST format. Example: PARSE TEST ID=123 Name=Elvan Programme=CMS Mark=88\n");
                        }
                    }
                    else if (strncmp(subCommand, "SAVE", 4) == 0) {
                        if (save_records(DB_NAME, head)) {
                            isModified = 0;
                            printf("CMS: Changes saved to \"%s\" successfully.\n", DB_NAME);
                        } else {
                            printf("CMS: Failed to save changes to \"%s\".\n", DB_NAME);
                        }
                    }
                    else {
                        handle_unknown(subCommand);
                    }
                }
            } else {
                printf("CMS: Failed to open database file \"%s\".\n", DB_NAME);
            }
        }
        else if (strcmp(userCommand, "QUIT") == 0) {
            printf("Exiting the CMS Database System. Goodbye!\n");
            return 0;
        }
        else {
            handle_unknown(userCommand);
        }
    }

    return 0;
}