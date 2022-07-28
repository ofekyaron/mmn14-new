/*
linked_list_utils: all the functions that needs to work with the node - linked list object
author: Ofek Yaron and Yuval Nisanov
*/

#include "linked_list_utils.h"

/*
push: Function to insert a new node on the front of the list.
Returns: New node header
 */
Node * push(struct Node** head_ref, void * new_data){
    /* allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    /* put in the data  */
    new_node->data  = new_data;
    /* Make next of new node as head */
    new_node->next = (*head_ref);
    /* move the head to point to the new node */
    (*head_ref) = new_node;
    return new_node;
}


/*
find_in_list: function run as recursion on linked list and search for a specific struct value in a node.
returns: founded struct data object.
 */
void* find_in_list(Node *node, void *search_value, int (*compare_func)(void  *data, void  *search_value)){
    int found;
//    print_list(node);
    while (node != NULL){
        found = compare_func(node->data,search_value);

        if (found){
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}

/*
print_list: This function prints contents of linked list starting from the given node
returns: void.
 */
void print_list(struct Node* n)
{
    while (n != NULL) {
        printf(" %s ", n->data);
        n = n->next;
    }
}
