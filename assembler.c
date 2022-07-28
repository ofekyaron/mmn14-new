/*
assembler: Program for compiling an assembly programs.
author: Ofek Yaron and Yuval Nisanov
*/
#include "preprocessor.h"
#include "first_scan.h"
#include "symbol_table.h"
#include "second_scan.h"
#include "output.h"
#include "queue.h"
#include "compilation.h"

int main(int argc, char *argv[]) {
    if (run_preprocessor("IO/input1.as", "input1.am") != 0)
        return FAILURE;
    return 0;

    int i, res;
    FILE *input_file;

    /* Checks that the number of arguments that were given is valid. */
    if (argc < MIN_ARG) {
        fprintf(stderr, "\nThe program must get at least 1 argument.\n");
        fprintf(stderr, "Usage: ./assembler <first_as_filename> <second_as_filename> ... \n");
        fprintf(stderr, "       The filenames should be given without the <as> extension.\n\n");
        return FAILURE;
    }


    /* Executes the compile method on each file which can be opened. */
    for (i = 1; i < argc; i++) {
        char *file_name = (char*)malloc(sizeof(argv[i]));
        char *clean_file_name = (char*)malloc(sizeof(argv[i]));
        if (file_name == NULL || clean_file_name == NULL) {
            fprintf(stderr, "Can't get memory for file names. The program is stopped.");
            return FAILURE;
        }
        strcpy(file_name, argv[i]);
        strcpy(clean_file_name, argv[i]);
        if ((input_file = fopen(strcat(file_name,".as"), "r")) == NULL) {
            fprintf(stderr, "The file <%s> can't be opened, please check that the file exists.\n"
                            "Skipping to the next file.\n\n", file_name);
        } else {
            printf("----------%s----------\n", file_name);
            res = compile(input_file, file_name, clean_file_name);
            fclose(input_file);
        }
        free(file_name);
        free(clean_file_name);
    }
    return res;
}

/*
compile: Compiling the assembly program, by 2 scans:
         first for filling the symbol list,
         second for parsing and decoding the lines.
         Finally creates the outputs:
         .ob file with the decoded code,
         .ent file with the entries,
         .ext file with the externals.
*/
int compile(FILE *input_file, char *file_name, char *clean_file_name) {
    int res;
    compilation_info info = {0};
    char *line_to_check;
    action action_list[] = {
            {"mov", 0, has_src, regular_src_dest},
            {"cmp", 1, has_src, regular_src_immediate_dest},
            {"add", 2, has_src, regular_src_dest},
            {"sub", 3, has_src, regular_src_dest},
            {"not", 4, no_src, regular_dest},
            {"clr", 5, no_src, regular_dest},
            {"lea", 6, src_only_label, src_only_label_regular_dest},
            {"inc", 7, no_src, regular_dest},
            {"dec", 8, no_src, regular_dest},
            {"jmp", 9, no_src, regular_dest},
            {"bne", 10, no_src, regular_dest},
            {"red", 11, no_src, regular_dest},
            {"prn", 12, no_src, immediate_dest},
            {"jsr", 13, no_src, regular_dest},
            {"rts", 14, no_src_no_dest, no_src_dest},
            {"stop", 15, no_src_no_dest, no_src_dest},
            {"not_valid",-1, NULL}
    };

    reg reg_list[] = {
            {"r0",0},
            {"r1",1},
            {"r2",2},
            {"r3",3},
            {"r4",4},
            {"r5",5},
            {"r6",6},
            {"r7",7},
            {"not_valid",-1}
    };

    command cmd_list[] = {
            {"data", parse_data},
            {"string", parse_string},
            {"struct", parse_struct},
            {"entry", check_entry},
            {"extern", parse_extern},
            {"not_valid", NULL}
    };

    info.action_list = action_list;
    info.reg_list = reg_list;
    info.cmd_list = cmd_list;

    /* preprocessor */
    char* file_am = (char*)malloc(sizeof (file_name) + 5);
    strcat(strcpy(file_am, file_name), ".am");
    run_preprocessor(&info, file_name, file_am);

    /* first scan */
    line_to_check = fgets(info.line, MAX_LINE_LENGTH, input_file);
    while (line_to_check != NULL)  {
        parse_line_first(&info);
        line_to_check = fgets(info.line, MAX_LINE_LENGTH, input_file);
        info.line_count++;
    }
    fix_symbol_values(info.symbol_head, info.instruction_count);

    if ((input_file = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "The file <%s> can't be opened, please check that the file exists.\n"
                        "Skipping to the next file.\n\n", file_name);
        return FAILURE;
    }

    /* second scan */
    info.line_count = 0;
    info.instruction_count = FIRST_ADDRESS;
    line_to_check = fgets(info.line, MAX_LINE_LENGTH, input_file);

    while (line_to_check != NULL)  {
        parse_line_second(&info);
        line_to_check = fgets(info.line, MAX_LINE_LENGTH, input_file);
        info.line_count++;
    }
    fclose(input_file);

    /* create outputs */
    if (info.fail == 0) {
        printf("# finished successfully, create outputs...\n\n");
        create_output(&info, clean_file_name);
        res = SUCCESS;
    }
    else {
        printf("# finished with errors, no outputs.\n\n");
        res = FAILURE;
    }
    /* free the memory */
    delete_list(info.symbol_head);
    delete_queue(info.entry_head);
    delete_queue(info.extern_head);
    return res;
}
