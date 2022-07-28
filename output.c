/*
output: Creates the outputs of the program:
        .ob files with the decoded code (in base 32).
        .ent files with the entries.
        .ext files with the externals.
author: Ofek Yaron and Yuval Nisanov
*/

#include "output.h"

/*
create_output: Creates all the outputs of the program:
               .ob file with the decoded code,
               .ent file if there are entries command in the code,
               .ext file if there are externs command in the code
*/
int create_output(compilation_info *info, char *file_name) {
    create_code_file(info, file_name);
    
    if (info->entry_head != NULL) 
        create_details_file(info->entry_head, file_name, ENTRY_EXTENSION); 
    if (info->extern_head != NULL) 
        create_details_file(info->extern_head, file_name, EXTERN_EXTENSION); 

    return 0;
}

/*
create_code_file: Creates the file with the decoded code (.ob)
*/
int create_code_file(compilation_info *info, char *file_name) {
    int i;
    FILE *ob_file;

    if ((create_file(file_name, OB_EXTENSION, &ob_file)) == FAILURE)
        return FAILURE;

    fprintf(ob_file, "Base 32 Address\tBase 32 machine-code\n");
    write_to_output_in_base32(info->instruction_count - FIRST_ADDRESS, ob_file);
    fputc(' ', ob_file);
    write_to_output_in_base32(info->data_count, ob_file);
    fputc('\n', ob_file);

    for (i = FIRST_ADDRESS; i < info->instruction_count; i++) {
        fputc('\t', ob_file);
        write_to_output_in_base32(i, ob_file);
        fputc('\t', ob_file);
        write_to_output_in_base32(info->instruction[i], ob_file);
        fputc('\n', ob_file);
    } 
    for (i = 0; i < info->data_count; i++) {
        fputc('\t', ob_file);
        write_to_output_in_base32(info->instruction_count + i, ob_file);
        fputc('\t', ob_file);
        write_to_output_in_base32(info->data[i], ob_file);
        fputc('\n', ob_file);
    }
    fclose(ob_file);
    return SUCCESS;
}

/*
create_details_file: Creates file with details about the compilation. 
                     e.g entries file and externs file.
*/
int create_details_file(element *head, char *file_name, char *extension) {
    element *cur;
    FILE *detail_file;

    if ((create_file(file_name, extension, &detail_file)) == FAILURE)
        return FAILURE;
    cur = head;
    while (cur != NULL) {
        fprintf(detail_file, "%s\t", cur->name);
        write_to_output_in_base32(cur->value, detail_file);           
        fputc('\n', detail_file);
        cur = cur->next;
    }
    fclose(detail_file);

    return SUCCESS;
}

/*
create_file: Creates new file with the given file name and extension,
             and store it in the given FILE pointer.
*/
int create_file(char *file_name, char *extension, FILE **file_pnt) {
    FILE *input_file;
    char *new_file_name = (char*)malloc(sizeof(file_name));
    
    if (new_file_name == NULL) {
        fprintf(stderr, "Failed with getting memory for file name.");
        return FAILURE;
    }
    strcpy(new_file_name, file_name);
    if ((input_file = fopen(strcat(new_file_name, extension), "w")) == NULL) {
        fprintf(stderr, "Failed in creation of object file.");
        return FAILURE;
     } 
     *file_pnt = input_file;
     free(new_file_name);
     return SUCCESS;   
}

/* 
convert_to_base_32: Convert the given number in decimal-base to 32-base,
                    and write it to the output.
                    The number must be between 0-32767 (15 bits in binary-base).   
*/
int write_to_output_in_base32(unsigned int num, FILE *output) {

    char base32[] = "!@#$%^&<>abcdefghijklmnopqrstuv";
    char r[4] = "000";
    int i = 2;

    while (num/32 > 0) {
        r[i] = base32[num%32];
        i--;
        num = num / 32;
    }
    r[i] = base32[num];
    for (i = 0; i < 3; i++)
        fputc(r[i], output);
    return 0;
}

/*
print_error: Prints the given line and error to the <stderr>.
*/
int print_error(int line, char *msg) {
    fprintf(stderr, "ERROR in line %d: %s\n", line + 1, msg);
    return 0;
}
