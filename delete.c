#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "delete.h"          
#include "parser.h"          
#include "utilities.h"      


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

    Student tmp;
    tmp.id = -1; tmp.mark = -1.0f;
    tmp.name[0] = '\0'; tmp.programme[0] = '\0';

    if (!parse_fields(args, &tmp) || tmp.id == -1) {
        printf("CMS: Invalid DELETE format. Example: DELETE ID=2401234\n");
        return -1;
    }

    Student *cur = *head;
    while (cur && cur->id != tmp.id) cur = cur->next;
    if (!cur) {
        printf("CMS: The record with ID=%d does not exist.\n", tmp.id);
        return 0;
    }

    printf("CMS: Are you sure you want to delete record with ID=%d? Type \"Y\" to Confirm or type \"N\" to cancel.\n", tmp.id);
    char yn[16];
    if (!fgets(yn, sizeof(yn), stdin)) {
        printf("CMS: The deletion is cancelled.\n");
        return 0;
    }
    trim_eol(yn);
    to_upper(yn);

    if (yn[0] != 'Y') {
        printf("CMS: The deletion is cancelled.\n");
        return 0;
    }

    if (delete_record_by_id(head, tmp.id)) {
        printf("CMS: The record with ID=%d is successfully deleted.\n", tmp.id);
        return 1;
    } else {
        printf("CMS: The record with ID=%d does not exist.\n", tmp.id);
        return 0;
    }
}
