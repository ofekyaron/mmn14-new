#ifndef macro_file
#define macro_file
#include "linked_list_utils.h"
#include <string.h>

typedef struct macro {
    char*  name;
    char*  value;
} Macro;


void print_macro(void *m);
int compare_macro(void* m, void* n);
void free_macro (void *m);
Node * add_new_macro(Node * head_ref ,char* name, char* value);

#endif
