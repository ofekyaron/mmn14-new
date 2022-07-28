#include <stdio.h>
#include <string.h>
#include "assembler.h"
#include "compilation.h"

int check_label(char*, compilation_info*);
int parse_line_first(compilation_info*);
int parse_data(compilation_info*);
int parse_string(compilation_info*);
int check_entry(compilation_info*);
int parse_extern(compilation_info*);
int parse_struct(compilation_info*);
int has_src(compilation_info*);
int src_only_label(compilation_info*);
int no_src(compilation_info*);
int no_src_no_dest(compilation_info*);
int get_action(char*, action*);
int get_reg(char*, reg*);
int get_command(char*, command*);