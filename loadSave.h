#ifndef LOADSAVE_H
#define LOADSAVE_H

#include "studentFunctions.h"

int load_records(const char *filename, Student **head, char *tableName);
int save_records(const char *filename, Student *head);

#endif