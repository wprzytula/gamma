/** @file
 * Interfejs modułu udostępniającego grę gamma w trybie wsadowym.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 5.05.2020
 */

#ifndef GAMMA_BATCH_MODE_H
#define GAMMA_BATCH_MODE_H

#include <stdbool.h>
#include <stdint.h>
#include "gamma.h"
#include <stdio.h>


typedef enum {END, BLANK, ERROR, VALID} load_res;


static inline void line_error(unsigned line) {
    fprintf(stderr, "ERROR %u\n", line);
}


load_res load_line(unsigned *line, char buffer[], unsigned *buff_index);


bool tokenize_line(char *buffer, unsigned buff_len, char *command,
                   uint64_t *params, unsigned *params_num);

void interpret_statement(unsigned line, gamma_t *g, char command,
                         uint64_t *params, unsigned params_num);


void batch_mainloop(unsigned line, gamma_t *g);

#endif //GAMMA_BATCH_MODE_H
