#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "studentFunctions.h"
#include "parser.h"
#include "loadSave.h"
#include "utilities.h"
//#include "delete.h"

#define MAX_INPUT_LENGTH 100
#define DB_NAME "Sample-CMS.txt"

int main() {
    char userCommand[MAX_INPUT_LENGTH];
    char userCommand1[16];
    char userCommand2[129];
    char currentTableName[128] = "";
    Student *head = NULL;
    int isModified = 0;


    display_declaration();
    printf("Welcome to the CMS Database System.\n");

    while (1) {
        printf("\nEnter command OPEN \"test.txt\" to open a .txt file or QUIT to quit the program : ");
        if (fgets(userCommand, sizeof(userCommand), stdin) == NULL) {
            printf("Input error.\n");
            continue;
        }

        userCommand[strcspn(userCommand, "\n")] = 0;
        //to_upper(userCommand);
        trim_whitespace(userCommand);
        sscanf(userCommand, "%15s %128s", userCommand1, userCommand2);
        to_upper(userCommand1);


        if (strcmp(userCommand1, "OPEN") == 0) {
            if (load_records(userCommand2, &head, currentTableName)) {
                printf("The database file \"%s\" is successfully opened.\n", userCommand2);

                char subCommand[MAX_INPUT_LENGTH];
                while (1) {
                    display_menu();
                    if (fgets(subCommand, sizeof(subCommand), stdin) == NULL) {
                        printf("Input error.\n");
                        continue;
                    }

                          subCommand[strcspn(subCommand, "\n")] = 0;
                          trim_whitespace(subCommand);
                          /* create an uppercase copy for case-insensitive command detection
                              while keeping original `subCommand` intact for parsing arguments */
                          char subUpper[MAX_INPUT_LENGTH];
                          strncpy(subUpper, subCommand, sizeof(subUpper)-1);
                          subUpper[sizeof(subUpper)-1] = '\0';
                          to_upper(subUpper);

                          if (strncmp(subUpper, "SHOW ALL", 8) == 0) {
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
                            show_all_records(head, currentTableName);
                        }
                    }
                    else if (strcmp(subUpper, "QUIT") == 0) {
                        if (isModified) {
                            char resp[8];
                            printf("You have unsaved changes. Save before exit? (Y/N): ");
                            if (fgets(resp, sizeof(resp), stdin) != NULL) {
                                if (toupper((unsigned char)resp[0]) == 'Y') {
                                    if (save_records(userCommand2, head)) {
                                        isModified = 0;
                                        printf("CMS: Changes saved to \"%s\" successfully.\n", userCommand2);
                                    } else {
                                        printf("CMS: Failed to save changes to \"%s\".\n", userCommand2);
                                    }
                                }
                            }
                        }
                        printf("Exiting the CMS Database System. Goodbye!\n");
                        return 0;
                    }
                    else if (strncmp(subUpper, "INSERT", 6) == 0) {
                        if (strlen(subCommand) > 7) {
                            const char *insertArgs = subCommand + 7;
                            insert_record(insertArgs, &head);
                            isModified = 1;
                        } else {
                            printf("CMS: Invalid INSERT format. Example: INSERT ID=123 Name=Elvan Programme=CMS Mark=88\n");
                        }
                    }
                    else if (strncmp(subUpper, "QUERY", 5) == 0) {
                        if (strlen(subCommand) > 6) {
                            const char *queryArgs = subCommand + 6;
                            query_record(queryArgs, head);
                        } else {
                            printf("CMS: Invalid QUERY format. Example: QUERY ID=123\n");
                        }
                    }
                    else if (strncmp(subUpper, "UPDATE", 6) == 0) {
                        if (strlen(subCommand) > 7) {
                            const char *updateArgs = subCommand + 7;
                            update_record(updateArgs, head);
                            isModified = 1;
                        } else {
                            printf("CMS: Invalid UPDATE format. Example: UPDATE ID=123 Programme=NewProgramme Mark=85.5\n");
                        }
                    }
                    else if (strncmp(subUpper, "DELETE", 6) == 0) {
                        if (strlen(subCommand) > 7) {
                            const char *delArgs = subCommand + 7;  
                            int res = delete_command(delArgs, &head);
                            if (res == 1) {
                                isModified = 1; 
                            }
                        } else {
                            printf("CMS: Invalid DELETE format. Example: DELETE ID=2401234\n");
                        }
                    }
                    else if (strcmp(subUpper, "SHOW SUMMARY") == 0) {
                        show_summary(head);
                    } 
                    else if (strncmp(subUpper, "PARSE TEST", 10) == 0) {
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
                    else if (strncmp(subUpper, "SAVE", 4) == 0) {
                        if (save_records(userCommand2, head)) {
                            isModified = 0;
                            printf("CMS: Changes saved to \"%s\" successfully.\n", userCommand2);
                        } else {
                            printf("CMS: Failed to save changes to \"%s\".\n", userCommand2);
                        }
                    }
                    else {
                        handle_unknown(subCommand);
                    }
                }
            } else {
                printf("CMS: Failed to open database file \"%s\".\n", userCommand2);
            }
        }
        else if (strcmp(userCommand1, "QUIT") == 0) {
            printf("Exiting the CMS Database System. Goodbye!\n");
            return 0;
        }
        else {
            handle_unknown(userCommand1);
        }
    }

    return 0;
}