//
// Created by Ofek Yaron on 26/07/2022.
//

#ifndef MMN14_OUTPUT_H
#define MMN14_OUTPUT_H
#include "assembler.h"
#include "compilation.h"

/* extensions */
#define OB_EXTENSION ".ob"
#define ENTRY_EXTENSION ".ent"
#define EXTERN_EXTENSION ".ext"

int create_output(compilation_info*, char*);
int create_file(char*, char*, FILE**);
int create_code_file(compilation_info*, char*);
int create_details_file(element*, char*, char*);
int write_to_output_in_base32(unsigned int, FILE*);
int print_error(int, char*);

#endif //MMN14_OUTPUT_H
