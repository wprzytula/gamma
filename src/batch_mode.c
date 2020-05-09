/** @file
 * Implementacja modułu udostępniającego grę gamma w trybie wsadowym.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 5.05.2020
 */

#include "batch_mode.h"
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "gamma.h"

#define MAX_PARAMS 4

load_res load_line(unsigned *line, char buffer[], unsigned *buff_index) {
    int chr;
    ++*line;
    if (feof(stdin))
        return END;

    *buff_index = 0;
    buffer[50] = '\0';

    while (!feof(stdin)) {
        if (*buff_index == 100) {
            while (getchar() != '\n' || feof(stdin));
            return ERROR;
        }
        chr = getchar();

        if (chr == -1) {
            fprintf(stderr, "%d\n", *buff_index);
            if (getchar(), !feof(stdin) || *buff_index != 0)
                return ERROR;
            else
                return END;
        }
        if (chr == '\n') {
            if (*buff_index == 0) {
                return BLANK;
            }
            else {
                buffer[(*buff_index)++] = '\0';
                return VALID;
            }
        }
        if (chr == '#' && *buff_index == 0) {
            while (getchar() != '\n') {
                if (feof(stdin))
                    return ERROR;
            }
            return BLANK;
        }
        if (isspace(chr)) {
            if (*buff_index == 0)
                return ERROR;
            else if (buffer[*buff_index - 1] == ' ')
                continue;
            else
                buffer[(*buff_index)++] = ' ';
        }
        else {
            buffer[(*buff_index)++] = chr;
        }
    }
    return ERROR;
}

static bool verify_as_uint32_t(uint64_t number) {
    bool res = number == (uint64_t)(uint32_t)number;
    return res ? res : puts("TOO BIG NUMBER"), res;
}

bool tokenize_line(char *buffer, unsigned buff_len, char *command,
                   uint64_t *params, unsigned *params_num) {
    // strip trailing space, if present
    if (buff_len >= 2 && buffer[buff_len - 1] == ' ')
        buffer[buff_len - 1] = '\0';
    *params_num = 0;

    *command = buffer[0];
    if (!isalpha(*command))
        return false;

    if (buff_len == 2)
        return true;

    if (buff_len >= 3 && !isspace(buffer[1]))
        return false;

    char *current_token = buffer + 2;
    char *next_token;
    while (*current_token != '\0') {
        errno = 0;
        uint64_t parsed_num = strtol(current_token, &next_token, 10);
        if (errno == ERANGE || current_token == next_token) {
            errno = 0;
            return false;
        }
        if (!verify_as_uint32_t(parsed_num))
            return false;
        params[(*params_num)++] = parsed_num;
        current_token = next_token;
        if (*params_num > MAX_PARAMS)
            return false;
    }
    return true;
}


typedef enum {MOVE = 'm', GOLDEN = 'g', BUSY = 'b',
              FREE = 'f', POSSIBLE = 'q', BOARD = 'p'}
              line_res;

void interpret_statement(unsigned line, gamma_t *g, char command,
                         uint64_t *params, unsigned params_num) {
    switch(command) {
        case MOVE:
            if (params_num != 3) {
                line_error(line);
                return;
            }
            else {
                printf("%i\n",
                        gamma_move(g, params[0], params[1], params[2]));
                return;
            }
        case GOLDEN:
            if (params_num != 3) {
                line_error(line);
                return;
            }
            else {
                printf("%i\n",
                       gamma_golden_move(g, params[0], params[1], params[2]));
                return;
            }
        case BUSY:
            if (params_num != 1) {
                line_error(line);
                return;
            }
            else {
                printf("%lu\n",
                       gamma_busy_fields(g, params[0]));
                return;
            }
        case FREE:
            if (params_num != 1) {
                line_error(line);
                return;
            }
            else {
                printf("%lu\n",
                       gamma_free_fields(g, params[0]));
                return;
            }
        case POSSIBLE:
            if (params_num != 1) {
                line_error(line);
                return;
            }
            else {
                printf("%i\n",
                       gamma_golden_possible(g, params[0]));
                return;
            }
        case BOARD:
            if (params_num != 0) {
                line_error(line);
                return;
            }
            else {
                char *board = gamma_board(g);
                printf("%s", board);
                free(board);
                return;
            }
        default:
            line_error(line);
    }
}

void batch_mainloop(unsigned line, gamma_t *g) {
    char buffer[100];
    unsigned buff_len;
    load_res load_result;
    char command;
    uint64_t params[4];
    unsigned params_num;

    do {
        load_result = load_line(&line, buffer, &buff_len);
        if (load_result == BLANK)
            continue;
        if (load_result == ERROR) {
            line_error(line);
            continue;
        }
        if (load_result == END)
            return;

     /*   puts("Buffer contains:");
        puts(buffer);
*/
        bool parsing_result = tokenize_line(buffer, buff_len, &command,
                                            params, &params_num);
       /* if (parsing_result) {
            puts("parsing succeeded");
            for (unsigned i = 0; i < params_num; ++i) {
                printf("%lu\n", params[i]);
            }
        }
        else
            puts("Error parsing");*/
        if (parsing_result)
            interpret_statement(line, g, command, params, params_num);
    } while (!feof(stdin));
}

