/*
symbol_list: Functions for dealing with the sybmol list.
author: uri hetzrony 
*/
#include "symbol_table.h"

/*
add_symbol: Adding new symbol to the list.
            Return the new head of the list.
*/
symbol *add_symbol(symbol *head, char *name, int value, int type) {
    if (head == NULL) {
        head = (symbol *)malloc(sizeof(symbol));
        if (head == NULL) {
            fprintf(stderr, "Failed in getting memory for the new symbol, "
                            "the results of the program certainly be wrong.");
            return NULL;
        }
        head->next = NULL;
        strcpy(head->name,name);
        head->value = value;
        head->type = type;
    } else {
        symbol *temp = (symbol *)malloc(sizeof(symbol));
        if (temp == NULL) {
            fprintf(stderr, "Failed in getting memory for the new symbol, "
                            "the results of the program certainly be wrong.");
            return NULL;
        }
        strcpy(temp->name,name);
        temp->value = value;
        temp->type = type;
        temp->next = head;
        head = temp;
    }
   return head;
}

/*
fix_symbol_values: Fixes the values and types of the symbols in the list
                   after the first scan.
*/ 
int fix_symbol_values(symbol *head, int instruction_count) {
    symbol *cur;

    cur = head;
    while (cur != NULL) {
        if (cur->type != EXTERNAL)
            cur->value += FIRST_ADDRESS;
        if (cur->type == DATA) {
            cur->value += instruction_count;
            cur->type = RELOCATABLE;
        }
        cur = cur->next;
    }
    return SUCCESS;
}

/*
get_symbol: Gets symbol from the list with the given name.
            Return null whether the symbol doesn't exist.
*/
symbol *get_symbol(symbol *head, char *name) {
    symbol *cur;

    cur = head;
    while (cur != NULL) {
        if (strcmp(name, cur->name) == 0) 
            return cur;
        cur = cur->next;
    }
    return NULL;
}

/*
get_not_external_symbol_by_index: Gets the symbol from the given index,
                                  from the sybmols that aren't external.
*/
symbol *get_not_external_symbol_by_index(symbol *head, int index) {
    int i = -1;
    symbol *cur;

    cur = head;
    while (cur != NULL && i < index) {
        if (cur->type != EXTERNAL)
            i++;
        cur = cur->next;
    }
    return cur;
}

/*
delete_list: Deletes all the elements of the list to free the memory.
*/
int delete_list(symbol *head) {
    symbol *cur;
    symbol *next;

    cur = head;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    return 0;
}
