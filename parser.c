#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"

void trim_whitespace(char *str)
{
    char *start = str;
    char *end;

    // Find first non-space character
    while (isspace((unsigned char)*start)) start++;

    // If the string is all spaces, make it empty
    if (*start == '\0')
    {
        *str = '\0';
        return;
    }

    // Trim trailing space from the trimmed start
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    // If there were leading spaces, shift the trimmed content to the front
    if (start != str)
    {
        memmove(str, start, (end - start) + 2); // include null terminator
    }
}


int parse_fields(const char *input, Student *tempStudent)
{
    if (!tempStudent || !input) return 0; //check if pointers are null

    //initialize student struct with default invalid values
    tempStudent->id = -1;
    tempStudent->mark = -1;
    tempStudent->name[0] = '\0';
    tempStudent->programme[0] = '\0';


    //copying the real input to tempinput for manipulation and removing the back newline character
    char tempInput[256];
    strncpy(tempInput, input, sizeof(tempInput));
    tempInput[sizeof(tempInput) - 1] = '\0';

    //locating the fields in the input string
    char *idPtr = strstr(tempInput, "ID=");
    char *namePtr = strstr(tempInput, "Name=");
    char *progPtr = strstr(tempInput, "Programme=");
    char *markPtr = strstr(tempInput, "Mark=");

    //strict ID parsing for integer to accept clean inputs only
    if (idPtr) {
        char *endptr;
        long id = strtol(idPtr + 3, &endptr, 10);
        if (*endptr == '\0' || isspace(*endptr)) {
            tempStudent->id = (int)id;
        } else {
            return 0;
        }
    }


    //strict Mark parsing for float to accept clean inputs only
    if (markPtr) {
        char *endptr;
        float mark = strtof(markPtr + 5, &endptr);
        if (*endptr == '\0' || isspace(*endptr)) {
            tempStudent->mark = mark;
        } else {
            return 0;
        }
    }

    //name extraction
    if (namePtr) {
        char *end = progPtr ? progPtr : markPtr ? markPtr : tempInput + strlen(tempInput);
        int len = end - (namePtr + 5);
        if (len > 0 && len < sizeof(tempStudent->name)) {
            strncpy(tempStudent->name, namePtr + 5, len);
            tempStudent->name[len] = '\0';
        }
    }

    //programme extraction
    if (progPtr) {
        char *end = markPtr ? markPtr : tempInput + strlen(tempInput);
        int len = end - (progPtr + 10);
        if (len > 0 && len < sizeof(tempStudent->programme)) {
            strncpy(tempStudent->programme, progPtr + 10, len);
            tempStudent->programme[len] = '\0';
        }
    }

    /*
    // Final validation
    if (tempStudent->id == -1 || tempStudent->mark < 0 ||
        strlen(tempStudent->name) == 0 || strlen(tempStudent->programme) == 0) {
        return 0;
    }
    */

    return 1;
}