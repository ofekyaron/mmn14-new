/*
second_scan: The second scanning of the compiler.
             Decodeing all the actions, and build 
             the entry and extern tables
author: uri hetzrony 
*/

#include "assembler.h"
#include "second_scan.h"

/*
parse_line_second: Final parsing and decoding for each lines.
                   Builds the instructions list, entries list,
                   and externals list.
*/
int parse_line_second(compilation_info *info) {
    int command_length, i;
    char *command;

    command = strtok(info->line, WHITESPACE_TOK);
    if (command != NULL && command[0] != ';') {
        command_length = strlen(command);
        if (command[command_length-1] == ':')
            command = strtok(NULL, WHITESPACE_TOK);
        if (command != NULL) {
            if (command[0] == '.') {
                command = &command[1];
                if (strcmp(command, "entry") == 0)
                    parse_entry(info);
            } else {
                i = get_action(command, info->action_list);
                if (info->action_list[i].code != -1)
                    (*(info->action_list[i]).parse_func)(info, info->action_list[i].code);
            }
        } 
    }
    return 0;
}

/*
parse_entry: Checks that the entry is valid - exists in symbol list,
             doesn't exist yet in the entries list and doesn't external.
*/
int parse_entry(compilation_info *info) {
    char *entry;
    symbol *entry_l;

    entry = strtok(NULL, WHITESPACE_TOK);
    if (entry == NULL) {
        info->fail = 1;
        print_error(info->line_count, ".entry command must get an argument.");
        return 0;
    }
    entry_l = get_symbol(info->symbol_head, entry);
    if (entry_l == NULL) {
        info->fail = 1;
        print_error(info->line_count, "This symbol doesn't exist.");
        return 0;
    } 
    if (entry_l->type == EXTERNAL) {
        info->fail = 1;
        print_error(info->line_count, ".entry commant can't get an external symbol as an argument.");
        return 0;
    }
    if (get_element(info->entry_head, entry) != NULL) {
        info->fail = 1;
        print_error(info->line_count, "This .entry command already exist.");
        return 0;
    }
    info->entry_tail = enqueue(&info->entry_head, info->entry_tail, entry, entry_l->value);

    if (strtok(NULL, WHITESPACE_TOK) != NULL) {
        info->fail = 1;
        print_error(info->line_count, ".entry command get only one argument.");
    }
    return 0;
}

/*
regular_src_dest: Decode action with regular src (immediate/direct/random/register)
                  and regular dest (direct/register).
*/
int regular_src_dest(compilation_info *info, int code) {
    char *first_arg, *second_arg;
    int reg = 0;
    int code_byte = info->instruction_count;

    first_arg = strtok(NULL, ", \t\n");
    second_arg = strtok(NULL, WHITESPACE_TOK);

    two_arguments_start(info, first_arg, second_arg);

    info->instruction[code_byte] |= (code << OPCODE);
    reg = parse_regular_src(info, first_arg, code_byte);
    
    info->instruction_count++;
    if (is_direct(info, second_arg)) { 
        info->instruction[code_byte] |= (DIRECT << DST);
    } else {
        if (reg) 
            info->instruction_count--;
        if (is_register(info, second_arg, REGULAR_EXTRA_BYTE))
            info->instruction[code_byte] |= (REGISTER << DST);
        else {
            info->fail = 1;
            print_error(info->line_count, "Invalid dest argument.");
        }
    }
    info->instruction_count++;
    two_arguments_end(info);
    return 0;
} 

/*
regular_src_immediate_dest: Decode action with regular src (immediate/direct/random/register)
                            and regular dest (direct/register) or immediate dest.
*/
int regular_src_immediate_dest(compilation_info *info, int code) {
    char *first_arg, *second_arg;
    int reg = 0;
    int code_byte = info->instruction_count;

    first_arg = strtok(NULL, ", \t\n");
    second_arg = strtok(NULL, WHITESPACE_TOK);

    two_arguments_start(info, first_arg, second_arg);
    
    info->instruction[code_byte] |= (code << OPCODE);
    reg = parse_regular_src(info, first_arg, code_byte);
    
    info->instruction_count++;
    if (second_arg[0] == '#') {
        info->instruction[code_byte] |= (IMMEDIATE << DST);
        immediate(info, second_arg);
    } else if (is_direct(info, second_arg)) { 
        info->instruction[code_byte] |= (DIRECT << DST);
    } else {
        if (reg) {
            info->instruction_count--;
            if (is_register(info, second_arg, REGULAR_EXTRA_BYTE))
                info->instruction[code_byte] |= (REGISTER << DST);
            else {
                info->fail = 1;
                print_error(info->line_count, "Invalid dest argument.");
            }
        }
    }
    
    info->instruction_count++;
    two_arguments_end(info);
    return 0;
}

/*
parse_regular_src: Parsing arg that can be from all the types:
                   immediate, direct, random or register.
*/
int parse_regular_src(compilation_info *info, char *arg, int code_byte) {
    int reg = 0;

    info->instruction_count++;
    if (arg[0] == '*') {
        info->instruction[code_byte] |= (RANDOM << SRC);
    } else if (arg[0] == '#') {
        info->instruction[code_byte] |= (IMMEDIATE << SRC);
        immediate(info, arg);
    } else if (is_direct(info, arg)) { 
        info->instruction[code_byte] |= (DIRECT << SRC);
    } else if (is_register(info, arg, SRC_REG_EXTRA_BYTE)) {
        info->instruction[code_byte] |= (REGISTER << SRC);
        reg = 1;
    } else {
        info->fail = 1;
        print_error(info->line_count, "Invalid src argument.");
    }
    return reg;
}

/*
src_only_label_regular_dest: Decode action with only label as src arg,
                             and regular dest arg - label or register.
*/
int src_only_label_regular_dest(compilation_info *info, int code) {
    char *first_arg, *second_arg;
    int type;
    int code_byte = info->instruction_count;

    first_arg = strtok(NULL, ", \t\n");
    second_arg = strtok(NULL, WHITESPACE_TOK);

    two_arguments_start(info, first_arg, second_arg);
    
    info->instruction[code_byte] |= (code << OPCODE);

    info->instruction_count++;
    if (strcmp(first_arg, "***") == 0) {
        info->instruction[code_byte] |= (RANDOM << SRC);
        random_label(info);
    } else {
        if (is_direct(info, first_arg))
            info->instruction[code_byte] |= (DIRECT << SRC);
        else {
            info->fail = 1;
            print_error(info->line_count, "This action must get label as a src arg.");
        }
    }
    
    info->instruction_count++;
    type = direct_and_register(info, second_arg);
    info->instruction[code_byte] |= (type << DST);
    
    info->instruction_count++;
    two_arguments_end(info);
    return 0;
}

/*
immediate_dest: Decode actions without src arg, and with
                regular dest arg - label or register, or direct dest arg.
*/
int immediate_dest(compilation_info *info, int code) {
    char *arg;
    int type;
    int code_byte = info->instruction_count;

    arg = strtok(NULL, WHITESPACE_TOK);
    one_argument_start(info, arg);
    
    info->instruction[code_byte] |= (code << OPCODE);
    info->instruction_count++;
    if (arg[0] == '#') {
        info->instruction[code_byte] |= (IMMEDIATE << DST);
        immediate(info, arg);
    } else {
        type = direct_and_register(info, arg);
        info->instruction[code_byte] |= (type << DST);
    }
    info->instruction_count++;
    one_argument_end(info);
    return 0;
}

/*
regular_dest: Decode actions without src arg, and with
              regular dest arg - label or register.
*/
int regular_dest(compilation_info *info, int code) {
    char *arg;
    int type;
    int code_byte = info->instruction_count;

    arg = strtok(NULL, WHITESPACE_TOK);
    one_argument_start(info, arg);

    info->instruction[code_byte] |= (code << OPCODE);
    info->instruction_count++;
    type = direct_and_register(info, arg);
    info->instruction[code_byte] |= (type << DST);
    
    info->instruction_count++;
    one_argument_end(info);
    return 0;
}

/*
no_src_dest: Decode action without any args.
*/
int no_src_dest(compilation_info *info, int code) {
    check_args(info, strtok(NULL, WHITESPACE_TOK) != NULL,
                            "This action doesn't get any arguments.");
    info->instruction[info->instruction_count] |= (code << OPCODE);
    info->instruction_count++;
    return 0;
}

/*
one_argument_end: Checks that actions with 1 arg don't get more than 1 arg.
*/
int one_argument_end(compilation_info *info) {
    return check_args(info, strtok(NULL, WHITESPACE_TOK) != NULL,
                            "This action gets only one argument.");
}

/*
one_argument_start: Checks that actions with 1 arg actually get 1 arg.
*/
int one_argument_start(compilation_info *info, char *arg) {
    return check_args(info, arg == NULL,
                           "This action must get one argument.");
}

/*
two_arguments_end: Checks that actions with 2 args don't gets more than 2 args.
*/
int two_arguments_end(compilation_info *info) {
    return check_args(info, strtok(NULL, WHITESPACE_TOK) != NULL,
                            "This action gets only two arguments.");
}

/*
two_arguments_start: Checks that actions with 2 args actually get 2 args.
*/
int two_arguments_start(compilation_info *info, char *first, char *second) {
    return check_args(info, first == NULL || second == NULL,
                           "This action must get two arguments.");
}

/*
check_args: If the given condition doesn't exist, print the given error.
*/
int check_args(compilation_info *info, int cond, char *error_msg) {
    if (cond) {
        info->fail = 1;
        print_error(info->line_count, error_msg);
    }
    return 0;
}

/*
immediate: If the arg is valid number - insert its value to an extra byte.
           Otherwise, print an error.
*/
int immediate(compilation_info *info, char *arg) {
    int value;
    char *ex;
    char check[1];    

    arg = &arg[1];
    value = (int)strtol(arg, &ex, DEFAULT_BASE);
    if (sscanf(ex,"%s",check) != -1) {
        info->fail = 1;
        print_error(info->line_count, "Illegal number.");
    } else {
        decode_extra_byte(info, arg, ABSOLUTE, value, REGULAR_EXTRA_BYTE);
    }
    return 0;
}

/*
direct_and_register: If the given arg is symbol or register,
                     insert their value to an extra byte and return the type.
                     Otherwise, print an error.
*/
int direct_and_register(compilation_info *info, char *arg) {

    if (is_direct(info, arg)) 
        return DIRECT;
    else if (is_register(info, arg, REGULAR_EXTRA_BYTE))
        return REGISTER;
    else {
        info->fail = 1;
        print_error(info->line_count, "Invalid var or register name.");
        return 0;
    }
}

/*
is_direct: If the given arg is a symbol in the symbol list, 
           decode an extra byte with its address.
           Otherwise return 0.
*/
int is_direct(compilation_info *info, char *arg) {
    symbol *arg_s;
    
    if ((arg_s = get_symbol(info->symbol_head, arg)) != NULL) {
        decode_extra_byte(info, arg, arg_s->type, arg_s->value, REGULAR_EXTRA_BYTE);
        return 1; 
    } else
        return 0;
}

/*
is_register: If the given arg is register, decode an extra byte with its value.
             Otherwise return 0.
*/
int is_register(compilation_info *info, char *arg, int extra_byte_offset) {
    int value;

    if ((value = get_reg(arg, info->reg_list)) != -1) {
        decode_extra_byte(info, arg, ABSOLUTE, value, extra_byte_offset);
        return 1; 
    } else
        return 0;
}

/*
decode_extra_byte: Decodes a byte in the instruction list 
                   according to the given details.
*/
int decode_extra_byte(compilation_info *info, char *arg, int era, int value, int offset) {
    info->instruction[info->instruction_count] |= era;
    if (era != EXTERNAL) 
        info->instruction[info->instruction_count] |= (value << offset);
    else
        info->extern_tail = enqueue(&info->extern_head, info->extern_tail, arg, info->instruction_count);
    return 0;
}

/*
parse_random: Gets a random number / register / internal label according
              to the arg and insert its value to an axtra byte.
              If the arg is invalid, print an error.
*/
int parse_random(compilation_info *info, char *arg) {
    int rnd, value;

    if (strcmp(arg,"*") == 0) {
        rnd = 1;
        value = get_random(NUM_OF_REGISTERS);
        decode_extra_byte(info, arg, ABSOLUTE, value, REGULAR_EXTRA_BYTE);
    } else if (strcmp(arg,"**") == 0) {
        rnd = 2;
        value = get_random(MAX_IMMEDIATE_NUM);
        decode_extra_byte(info, arg, ABSOLUTE, value, REGULAR_EXTRA_BYTE);
    } else if (strcmp(arg,"***") == 0) {
        rnd = 3;
        random_label(info);
    } else {
        info->fail = 1;
        print_error(info->line_count, "Invalid arg! Random can be only *, **, or ***");
        return 0;
    }
    return rnd;
}

/*
random_label: Gets random internal label and insert 
              its address to an extra code byte.
              If there isn't any internal label, print an error.
*/
int random_label(compilation_info *info) {
    symbol *rnd_s;
    int rnd;
    
    if (info->not_external_symbol_count > 0) {
        rnd = get_random(info->not_external_symbol_count); 
        rnd_s = get_not_external_symbol_by_index(info->symbol_head, rnd);
        decode_extra_byte(info, rnd_s->name, RELOCATABLE, rnd_s->value, REGULAR_EXTRA_BYTE);
    } else {
        info->fail = 1;
        print_error(info->line_count, "There isn't an internal label in this program.");
        return 0;
    }
    return 0;
}

/*
get_random: Gets random int between 0 and given max-1.
*/
int get_random(int max) {
    srand(time(NULL));
    return rand() % (max);
}
