/*
macro: macro methods used in node.
author: Ofek Yaron and Yuval Nisanov
*/

#include "macro.h"


/*
 * Function:  free_macro
 * --------------------
 * free all macro malloced data
 *
 * Returns: void
 */
void free_macro (void *m){
    Macro *macro=m;
    free(macro->value);
    free(macro->name);
    free(macro);
}


/*
 * Function:  print_macro
 * --------------------
 * print macro data
 *
 * Returns: void
 */
void print_macro(void *m){
    Macro *macro = m;
    printf("|\t%s\t",macro->name);
    printf("|\t%s\t|\n",macro->value);
}


/*
* Function:  compare_macro
* --------------------
* compare new macro base on the macro name, used to find macro in linked list
*
* Returns: 1 if macro is similar 0 if not.
*/
int compare_macro(void* m, void* n){
    Macro *macro=m;
    char* name = n;
    printf("macros: %s %s ", macro->name,name);
    if (strcmp(macro->name,name) == 0)
        return 1;
    return 0;
}

Node * add_new_macro(Node * head_ref ,char* name, char* value){
    Macro *p;
    char* macroValue;
    Macro macroV;
    char* macroName = (char*)malloc(sizeof(name)+1);
    if (name != NULL){
        strcpy(macroName,name);
    }

    macroValue = (char*)malloc(sizeof(value)+1);
    if (value != NULL){
        strcpy(macroValue,value);
    }
    macroV.name = macroName;
    macroV.value = macroValue;

    p = &macroV;
    return push(&head_ref, p);
}
