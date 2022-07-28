//
// Created by Ofek Yaron on 26/07/2022.
//

#ifndef MMN14_SYMBOL_TABLE_H
#define MMN14_SYMBOL_TABLE_H
#include "assembler.h"

symbol *add_symbol(symbol*, char*, int, int);
int fix_symbol_values(symbol*, int);
symbol *get_symbol(symbol*, char*);
symbol *get_not_external_symbol_by_index(symbol*, int);
int delete_list(symbol*);


#endif //MMN14_SYMBOL_TABLE_H
