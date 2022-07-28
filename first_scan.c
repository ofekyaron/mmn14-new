/*
first_scan: The first scanning of the compilation - for building the symbol list.
author: Ofek Yaron and Yuval Nisanov
*/

#include "first_scan.h"
#include "output.h"
#include "symbol_table.h"
/*
parse_line_first: First parsing of the line.
                  Geting the label if exists and checking it,
                  storing it in the symbol list, parsing the commands,
                  and counting the bytes for decoding the actions.
*/
int parse_line_first(compilation_info *info) {
    int command_length, action_index, ic;
    char *command;

    command = strtok(info->line, WHITESPACE_TOK); /*seperates the line by whitespace */
    if (command != NULL && command[0] != ';') { /*if command is not comment line */
        command_length = strlen(command);
        /* parsing the label if exists */
        info->has_label=FALSE;
        if (command[command_length-1] == ':') {
            command[command_length-1] = '\0';
            if (check_label(command, info)) {
                info->has_label = TRUE;
                strcpy(info->label, command);
            } else /* label is invalid */
                info->fail = 1;
            command = strtok(NULL, WHITESPACE_TOK);
        }
        if (command != NULL) {
            /* parsing the command if exists */
            if (command[0] == '.') {
                command = &command[1];
                action_index = get_command(command, info->cmd_list);
                if (info->cmd_list[action_index].func != NULL)
                    (*(info->cmd_list[action_index]).func)(info);
                else {
                    info->fail = 1;
                    print_error(info->line_count, "Illegal command.");
                }
                /* parsing the action if exists */
            } else {
                if (info->has_label) {
                    info->not_external_symbol_count++;
                    info->symbol_head = add_symbol(info->symbol_head, info->label, info->instruction_count, RELOCATABLE);
                }
                action_index = get_action(command, info->action_list);
                if (info->action_list[action_index].code != -1) {
                    ic = (*(info->action_list[action_index]).count_func)(info);
                    info->instruction_count += ic;
                }
                else {
                    info->fail = 1;
                    print_error(info->line_count, "Illegal action.");
                }
            }
        } else {
            info->fail = 1;
            print_error(info->line_count, "No command or action.");
        }
    }
    return 0;
}
/*
get_action: Gets the index of the given action, in the actions list.
*/
int get_action(char *value, action *action_list) {
    int i;
    for (i = 0; action_list[i].code != -1; i++)
        if (strcmp(value, action_list[i].command) == 0)
            break;
    return i;
}

/*
get_reg: Gets the number of the register with the given name.
*/
int get_reg(char *value, reg *reg_list) {
    int i;
    for (i = 0; reg_list[i].num != -1; i++)
        if (strcmp(value, reg_list[i].name) == 0)
            break;
    return reg_list[i].num;
}

/*
get_command: Gets the index of the given command, in the commands list.
*/
int get_command(char *value, command *cmd_list) {
    int i;
    for (i = 0; cmd_list[i].func != NULL; i++)
        if (strcmp(value, cmd_list[i].name) == 0)
            break;
    return i;
}

/*
check_label: Checks that the name of the label is legal -
             doesn't exist yet, contain only letters and number,
             start with letter, doesn't have name of action or register
             and isn't longer than 30 chars.
*/
int check_label(char *label, compilation_info *info) {
    int i;

    if (strlen(label) > MAX_LABEL_LENGTH) {
        print_error(info->line_count, "Label name is limited to 30 chars.");
        return 0;
    }
    if (label[0] < 'A' || label[0] > 'z') {
        print_error(info->line_count, "Label name must start with a letter.");
        return 0;
    }
    for (i = 1; label[i] != '\0'; i++) {
        if (label[i] < '0' ||
            (label[i] > '9' && label[i] < 'A') ||
            label[i] > 'z') {
            print_error(info->line_count, "Label name can contain only letters and numbers.");
            return 0;
        }
    }
    if (info->action_list[get_action(label, info->action_list)].code != -1) {
        print_error(info->line_count, "Opcode can't be a label name.");
        return 0;
    }

    if (get_reg(label, info->reg_list) != -1) {
        print_error(info->line_count, "Register name can't be a label name.");
        return 0;
    }

    if (get_symbol(info->symbol_head, label) != NULL) {
        print_error(info->line_count, "Label name already exists.");
        return 0;
    }

    return 1;
}

/*
is_valid_string: check if given char * is valid
*/
int is_valid_number(compilation_info *info, int num) {
    int num2;
    char *ex;
    char check[1];

    while (num != NULL) {
        num2 = (int)strtol(num, &ex, DEFAULT_BASE);
        if (sscanf(ex,"%s",check) != -1) {
            info->fail = 1;
            print_error(info->line_count, "Illegal number.");
            num = NULL;
        } else {
            info->data[info->data_count] = num2;
            info->data_count++;
            num = strtok(NULL,COMMA_TOK);
        }
    }
    return 0;
}



/*
parse_data: Parses the number from .data command and insert them to
            the data list.
*/
int parse_data(compilation_info *info) {
    char *num;
    char *ex;
    char check[1];
    int input;

    if (info->has_label) {
        info->not_external_symbol_count++;
        info->symbol_head = add_symbol(info->symbol_head, info->label, info->data_count, DATA);
    }
    num = strtok(NULL,COMMA_TOK);
    if (num == NULL) {
        info->fail = 1;
        print_error(info->line_count, ".data command must get at list 1 number.");
    }
    is_valid_number(info, num);
    return 0;
}

/*
is_valid_string: check if given char * is valid
*/
int is_valid_string(compilation_info *info, char *c, int i){
    while (c[i] == ' ' || c[i] == '\t')
        i++;
    if (c[i] == '\"') {
        i++;
        while (c[i] != '\0' && c[i] != '\"') {
            info->data[info->data_count] = c[i];
            i++;
            info->data_count++;
        }
        if (c[i] == '\"') {
            i++;
            info->data_count++;
        }
    }
    while (c[i] == ' ' || c[i] == '\t')
        i++;
    if (c[i] == '\n')
        return 1;
    print_error(info->line_count, "illegal string.");
    return 0;
}


/*
parse_string: Parses the string from .string command and insert
              the ascii chars to the data list.
*/
int parse_string(compilation_info *info) {
    int i = 0;
    int result = 0;
    char *input;

    if (info->has_label) {
        info->not_external_symbol_count++;
        info->symbol_head = add_symbol(info->symbol_head, info->label, info->data_count, DATA);
    }
    input = strtok(NULL, "");
    if (input != NULL)
        result = is_valid_string(info, input, i);
    if (result != 1) {
        info->fail = 1;
        print_error(info->line_count, ".data command must get only string between quotation marks.");
        return 0;
    } else return 0;
}

/*
check_entry: Checks that there isn't a label before the .entry command.
*/
int check_entry(compilation_info *info) {
    if (info->has_label == 1) {
        info->fail = 1;
        print_error(info->line_count, "Label before .entry command is illegal.");
    }
    return 0;
}

/*
parse_extern: Checks that there isn't a label before the .extern command,
              checks that the label is legal and insert it to the symbol list.
*/
int parse_extern(compilation_info *info) {
    char *label;

    if (info->has_label == 1) {
        info->fail = 1;
        print_error(info->line_count, "Label before .extern command is illegal.");
    }
    label = strtok(NULL, WHITESPACE_TOK);
    if (label == NULL) {
        info->fail = 1;
        print_error(info->line_count, ".extern command must get an argument.");
        return 0;
    }
    if (check_label(label, info)) {
        info->symbol_head = add_symbol(info->symbol_head, label, -1, EXTERNAL);
    } else
        info->fail = 1;

    if (strtok(NULL, WHITESPACE_TOK) != NULL) {
        info->fail = 1;
        print_error(info->line_count, ".extern command get only one argument.");
    }
    return 0;
}

/*
parse_struct: Parses the number from .data command and insert them to
            the data list.
*/
int parse_struct(compilation_info *info) {
    char *portion1, *portion2;
    int count=0;
    if (info->has_label) {
        info->not_external_symbol_count++;
        info->symbol_head = add_symbol(info->symbol_head, info->label, info->data_count, DATA);
    }
    portion1 = strtok(NULL, COMMA_TOK);
    
    while (portion1 != NULL){
        count++;
        portion1 = strtok(NULL, COMMA_TOK);
    }
    if (count != 2) {
        info->fail = 1;
        print_error(info->line_count, ".struct command must get exactly 2 values.");
    }

    portion2 = strtok(info->line,COMMA_TOK);
    is_valid_number(info,portion2);
    portion2 = strtok(info->line,COMMA_TOK);
    is_valid_string(info, portion2, 0);
    return 0;
    }

/*
has_src: Checks the number of extra bytes for command with 2 arguments.
         If the two arguments are registers, there is 1 extra bytes,
         otherwise 2.
*/
int has_src(compilation_info *info) {
    char *first_arg;
    char *second_arg;
    int first = 0, second = 0;

    first_arg = strtok(NULL, ", \t\n");
    second_arg = strtok(NULL, WHITESPACE_TOK);

    if (first_arg != NULL) {
        if (strcmp(first_arg, "*") == 0)
            first = 1;
        else {
            if (get_reg(first_arg, info->reg_list) != -1)
                first = 1;
        }
    }
    if (second_arg != NULL) {
        if (get_reg(second_arg, info->reg_list) != -1)
            second = 1;
    }
    if (first && second)
        return 2;
    else
        return 3;
}

/*
src_only_label: For actions that the src argument can't be a register,
                so they always have 2 extra bytes.
*/
int src_only_label(compilation_info *info) {
    return 3;
}

/*
no_src: For actions without src argument. Always 1 extra byte.
*/
int no_src(compilation_info *info) {
    return 2;
}

/*
no_src_no_dest: For actions without agruments. No extra bytes.
*/
int no_src_no_dest(compilation_info *info) {
    return 1;
}