//
// Created by Ofek Yaron on 26/07/2022.
//

#ifndef MMN14_QUEUE_H
#define MMN14_QUEUE_H
#include "assembler.h"

element *get_element(element*, char*);
element *enqueue(element**, element*, char*, int);
int delete_queue(element*);

#endif //MMN14_QUEUE_H
