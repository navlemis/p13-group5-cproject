#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "utilities.h"

// Converts a string to uppercase
void to_upper(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}

// Displays the CMS command menu
void display_menu() {
    printf("\n-------------------------------\n");
    printf("CMS Database System Commands:\n");
    printf("1. SHOW ALL - Display all contents in the database table.\n");
    printf("2. INSERT ID=(id) Name=(Name) Programme=(programme) Mark=(mark) - Insert a new record into the database.\n");
    printf("3. QUERY ID=(id) - Search for a student record by ID.\n");
    printf("4. UPDATE ID=(id) Programme=(programme to update) Mark=(mark to update) - Update Programme and/or Mark for an existing record.\n");
    printf("5. SAVE - Save changes to the database file after making edits.\n");
    printf("6. QUIT - Exit the program.\n");
    printf("Please enter a command (Case Sensitive): ");
}

// Displays the plagiarism declaration
void display_declaration() {
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

// Handles unknown commands
void handle_unknown(const char *command) {
    printf("Unknown command \"%s\". Please enter a valid command.\n", command);
}