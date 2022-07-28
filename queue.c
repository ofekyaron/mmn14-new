/*
queue: Functions for creating queue.
       Used for the enties and externals lists. 
author: Ofek Yaron and Yuval Nisanov
*/

#include "queue.h"

/*
enqueue: Insert new element to the list.
         Return the new tail of the list.
*/
element *enqueue(element **head, element *tail, char *name, int value) {
    if (tail == NULL) {
        tail = (element *)malloc(sizeof(element));
        if (tail == NULL) {
            fprintf(stderr, "Failed in getting memory for the new element in the list, "
                            "the results of the program certainly be wrong.");
            return NULL;
        }
        tail->next = NULL;
        strcpy(tail->name, name);
        tail->value = value;
        *head = tail;
    } else {
        element *temp = (element *)malloc(sizeof(element));
        if (temp == NULL) {
            fprintf(stderr, "Failed in getting memory for the new element in the list, "
                            "the results of the program certainly be wrong.");
            return NULL;
        }
        tail->next = temp;
        strcpy(temp->name, name);
        temp->value = value;
        temp->next = NULL;
 
        tail = temp;
    }
    return tail;
}

/*
get_element: Gets element from the list with the given name.
             Return NULL whether this element doesn't exist.
*/
element *get_element(element *head, char *name) {
    element *cur;

    cur = head;
    while (cur != NULL) {
        if (strcmp(name, cur->name) == 0) 
            return cur;
        cur = cur->next;
    }
    return NULL;
}

/*
delete_queue: Deletes all the elements of the queue to free the memory.
*/
int delete_queue(element *head) {
    element *cur;
    element *next;

    cur = head;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    return 0;
}
