#ifndef PARSER_H
#define PARSER_H

#include "studentFunctions.h"

int parse_fields(const char *input, Student *tempStudent);
void trim_whitespace(char *str);

#endif