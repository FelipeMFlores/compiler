#pragma once

// some args can be NULL
typedef struct iloc {
    char* operation;
    char* arg1;
    char* arg2;
    char* arg3;
} ILOC;

// doubly linked list. always use the tail of the list, to save time
typedef struct node_list {
    ILOC* iloc;
    struct node_list *prev;
    struct node_list *next;
} NODE_LIST;
