#ifndef linked_list_utils
#define linked_list_utils
#include<stdio.h>
#include<stdlib.h>


typedef struct Node {
    /* Any data type can be stored in this node */
    void  *data;
    struct Node* next;
} Node;



Node * push(struct Node** head_ref, void * new_data);

/*
void free_list(struct Node *node, void (*delMethod)(void *));

void print_list(Node *node);
*/
void* find_in_list(Node *node, void *search_value, int (*compare_func)(void  *data, void  *search_value));

void print_list(struct Node* n);

#endif
