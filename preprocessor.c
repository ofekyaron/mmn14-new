/*
preprocessor: Exchange macro name to matched string
author: Ofek Yaron and Yuval Nisanov
*/

#include "assembler.h"
#include "linked_list_utils.h"

typedef struct macro {
    char*  name;
    char*  value;
} Macro;


void print_macro(void *m);
int compare_macro(void* m, void* n);
void free_macro (void *m);
Node * add_new_macro(Node * head_ref ,char* name, char* value);

/*
macro: macro methods used in node.
author: Ofek Yaron and Yuval Nisanov
*/


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


/*
  run_preprocessor: function reads .as file line by line.
  founds start of macro - add the macro to the macro's table and read the content of the macro
  founds macro name - get the matched string from the macro's table
 */
int run_preprocessor(char as_file[], char am_file[]){
    char line[MAX_LINE_LENGTH];
    FILE *as_file_pointer;
    FILE *am_file_pointer;
    Node * macros_table_head = NULL;;

    as_file_pointer = fopen(as_file, "r");

    /* Validate as file opened successfully */
    if (!as_file_pointer){
        printf("Error: could not open file in path %s\n", as_file);
        perror("Caused by");
        return -1;
    }


    else { /* If file opened successfully */
        char macro_name[MAX_LINE_LENGTH];
        Macro* found_macro;
        Macro *current_macro = NULL;
        /* Create am file - source file after preprocessor step */
        am_file_pointer = fopen(am_file, "w");

        /* Validate if file was created*/
        if(am_file_pointer == NULL){
            printf("Unable to create file.\n");
            return -1;
        }

        /* If file was created successfully*/
        else {
            /* Run on each row in source file*/
            while(fgets(line, MAX_LINE_LENGTH, as_file_pointer) != NULL ) {
                char first_word[MAX_LINE_LENGTH];
                first_word[0] = '\0';
                sscanf(line, " %s ", first_word);

                found_macro = find_in_list(macros_table_head, first_word, compare_macro);
                if (found_macro != NULL){
                    fputs(found_macro->value, am_file_pointer);
                    continue;
                }

                /* creates the content of the new file - copy rows*/
                if (current_macro == NULL){ /* if not in macro*/
                    /* Validate if comment */
                    if(strstr(line, ";")){
                        fputs(line, am_file_pointer);
                        continue;
                    }

                    else if (!strcmp(first_word, "macro")) { /* if first word is macro */
                        sscanf(line, "%s %s ", first_word, macro_name);

                        current_macro = (Macro*)malloc(sizeof(Macro));
                        current_macro->name = strdup(macro_name);
                        current_macro->value = NULL;
                        macros_table_head = push(&macros_table_head, current_macro);
                    }

                    else {
                        fputs(line, am_file_pointer);

                    }
                }
                else { /* if in macro */
                    if(!strcmp(first_word, "endmacro")){ /* if end of macro */
                        current_macro = NULL;
                        continue;
                    }
                        if (current_macro->value == NULL) {
                            current_macro->value = strdup(line);
                        }
                        else {
                            char *new_value = (char*)malloc(sizeof(current_macro->value) + sizeof(line) +1);
                            strcpy(new_value, current_macro->value);
//                            strcat(new_value, "\n");
                            strcat(new_value, line);
//                            new_value[strlen(new_value)+1] = '\n';
//                            strcpy(new_value+ sizeof(current_macro->value) +3, line);
                            free(current_macro->value);
                            current_macro->value = new_value;
                        }
                        printf("current macro_val: %s", current_macro->value);
                    }
                } /* end of in macro */

            } /* end of while*/
        fclose(am_file_pointer);

        }
//    find_in_list(macros_table_head, first_word, free());

    //TODO free macros
        fclose(as_file_pointer);
    return 0;
}
