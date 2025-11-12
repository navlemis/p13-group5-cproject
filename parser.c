#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "parser.h"

int parse_fields(const char *input, Student *tempStudent) {
    if (!tempStudent) return 0;

    tempStudent->id = -1;
    tempStudent->mark = -1;
    tempStudent->name[0] = '\0';
    tempStudent->programme[0] = '\0';

    char buffer[256];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *idPtr = strstr(buffer, "ID=");
    char *namePtr = strstr(buffer, "Name=");
    char *progPtr = strstr(buffer, "Programme=");
    char *markPtr = strstr(buffer, "Mark=");

    if (idPtr) sscanf(idPtr, "ID=%d", &tempStudent->id);
    if (markPtr) sscanf(markPtr, "Mark=%f", &tempStudent->mark);

    if (namePtr) {
        char *end = progPtr ? progPtr : markPtr ? markPtr : buffer + strlen(buffer);
        int len = end - (namePtr + 5);
        if (len > 0 && len < sizeof(tempStudent->name)) {
            strncpy(tempStudent->name, namePtr + 5, len);
            tempStudent->name[len] = '\0';
        }
    }

    if (progPtr) {
        char *end = markPtr ? markPtr : buffer + strlen(buffer);
        int len = end - (progPtr + 10);
        if (len > 0 && len < sizeof(tempStudent->programme)) {
            strncpy(tempStudent->programme, progPtr + 10, len);
            tempStudent->programme[len] = '\0';
        }
    }

    return 1;
}