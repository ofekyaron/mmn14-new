//
// Created by Ofek Yaron on 28/07/2022.
//

#ifndef MMN14_COMPILATION_H
#define MMN14_COMPILATION_H

/* defines */
#define MIN_ARG 2
#define MAX_LINE_LENGTH 81
#define MAX_INSTRUCTION 256
#define MAX_DATA 256
#define FIRST_ADDRESS 100
#define MAX_LABEL_LENGTH 30
#define DATA -1
#define WHITESPACE_TOK " \t\n"
#define COMMA_TOK ","
#define NUM_OF_REGISTERS 8
#define MAX_IMMEDIATE_NUM 8192
#define DEFAULT_BASE 10

/* offsets */
#define DST 2
#define SRC 4
#define OPCODE 6
#define REGULAR_EXTRA_BYTE 2
#define SRC_REG_EXTRA_BYTE 8

/* enums */
enum{SUCCESS, FAILURE};
enum{FALSE, TRUE};
enum{ABSOLUTE, EXTERNAL, RELOCATABLE};
enum{NO_ARGS, ONE_ARG, TWO_ARGS};
enum{IMMEDIATE, DIRECT, RANDOM, REGISTER};

/* structs */

/* Element in a queue, for entry and extern list. */
typedef struct queue_element {
    char name[MAX_LABEL_LENGTH + 1];
    int value;
    struct queue_element *next;
} element;

/* Element in the symbol list. */
typedef struct symbol_element {
    char name[MAX_LABEL_LENGTH + 1];
    int value;
    int type;
    struct symbol_element *next;
} symbol;

/* For actions list. */
typedef struct action_func {
    char *command;
    int code;
    int (*count_func)(struct comp_info*);
    int (*parse_func)(struct comp_info*,int);
} action;

/* For registers list. */
typedef struct reg_info {
    char *name;
    int num;
} reg;

/* For command list. */
typedef struct command_func {
    char *name;
    int (*func)(struct comp_info*);
} command;



/* For storing info about the compiling. */
typedef struct comp_info {
    int instruction_count;
    int data_count;
    int not_external_symbol_count;
    int line_count;
    int has_label;
    int fail;
    int has_entry;
    int has_external;

    short int instruction[MAX_INSTRUCTION];
    short int data[MAX_DATA];
    char line[MAX_LINE_LENGTH + 1];
    char label[MAX_LABEL_LENGTH + 1];
    element *entry_head;
    element *entry_tail;
    element *extern_head;
    element *extern_tail;
    symbol *symbol_head;
    action *action_list;
    reg *reg_list;
    command *cmd_list;
} compilation_info;


#endif //MMN14_COMPILATION_H
