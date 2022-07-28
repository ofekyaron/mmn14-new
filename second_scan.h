//
// Created by Ofek Yaron on 26/07/2022.
//

#ifndef MMN14_SECOND_SCAN_H
#define MMN14_SECOND_SCAN_H
#include "assembler.h"
#include "compilation.h"

/* second_scan.c */
int parse_line_second(compilation_info*);
int parse_entry(compilation_info*);
int regular_src_dest(compilation_info*, int);
int regular_src_immediate_dest(compilation_info*, int);
int regular_dest(compilation_info*, int);
int immediate_dest(compilation_info*, int);
int src_only_label_regular_dest(compilation_info*, int);
int no_src_dest(compilation_info*, int);
int decode_extra_byte(compilation_info*, char*, int, int, int);
int direct_and_register(compilation_info*, char*);
int one_argument_end(compilation_info*);
int two_arguments_end(compilation_info*);
int get_random(int);
int immediate(compilation_info*, char*);
int parse_random(compilation_info*, char*);
int random_label(compilation_info*);
int check_args(compilation_info*, int, char*);
int one_argument_start(compilation_info*, char*);
int two_arguments_start(compilation_info*, char*, char*);
int is_direct(compilation_info*, char*);
int is_register(compilation_info*, char*, int);
int parse_regular_src(compilation_info*, char*, int);

#endif //MMN14_SECOND_SCAN_H
