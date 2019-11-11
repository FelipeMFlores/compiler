#include "codeGenerator.h"

#define MAX_SIZE 5 //1 char + 1 int


char* generate_label(){
    static int label_number = 0;
    char* new_label = malloc(MAX_SIZE); 
    snprintf(new_label, MAX_SIZE, "L%d", label_number++);
    return new_label;
}

char* generate_register(){
    static int register_number = 0;
    char* new_register = malloc(MAX_SIZE); 
    snprintf(new_register, MAX_SIZE, "r%d", register_number++);
    return new_register;
}
