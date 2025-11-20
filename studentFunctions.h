#ifndef STUDENT_FUNCTIONS_H
#define STUDENT_FUNCTIONS_H


//defining the student "object"
typedef struct Student { 
    int id;
    char name[88];
    char programme[88];
    float mark;
    struct Student *next;
} Student;



//calling the functions from .c counterpart
void insert_record(const char *args, Student **head);
void query_record(const char *args, Student *head);
void show_all_records(Student *head, char *tableName);
void show_all_sorted(Student *head, const char *field, const char *order);
void update_record(const char *args, Student *head);
int count_records(Student *head);
void show_summary(Student *head);
int delete_command(const char *args, Student **head);
void free_records(Student *head);


#endif



