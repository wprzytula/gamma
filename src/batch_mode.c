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

#define MAX_PARAMS 4  ///< maksymalna poprawna liczba parametrów polecenia

/** @brief Sprawdza czy liczba mieści się w zakresie @a uint32_t.
 * Rzutuje liczbę nieujemną z zakresu @a uint64_t na typ @a uint32_t oraz z powrotem,
 * a następnie sprawdza równość wyniku i wyjściowej wartości.
 * @param[in] number  -  sprawdzana liczba nieujemna.
 * @return Wartość @p true, jeśli dana liczba mieści się w zakresie @a uint32_t
 * lub @p false w przeciwnym przypadku.
 */
static inline bool verify_as_uint32_t(uint64_t number) {
    bool res = number == (uint64_t)(uint32_t)number;
    return res;
}

/**
 * Typ polecenia.
 */
typedef enum {MOVE = 'm', GOLDEN = 'g', BUSY = 'b',
    FREE = 'f', POSSIBLE = 'q', BOARD = 'p'}
        command_t;

load_res load_line(unsigned *line, char **buffer, size_t *buff_cap,
                   size_t *buff_len) {
    int chr;
    ++*line;
    *buff_len = 0;
    if (feof(stdin))
        return END;

    while (true) {
        if (*buff_cap == *buff_len) {
            char *new_buff = realloc(*buffer, sizeof(char) * (*buff_cap + 10));
            if (new_buff) {
                *buffer = new_buff;
                *buff_cap += 10;
            }
            else {
                return ERROR;
            }
        }

        chr = getchar();

        if (chr == -1) {
            if (getchar(), !feof(stdin) || *buff_len != 0)
                return ERROR;
            else
                return END;
        }
        if (chr == '\n') {
            if (*buff_len == 0) {
                return BLANK;
            }
            else {
                (*buffer)[(*buff_len)++] = '\0';
                return VALID;
            }
        }
        if (chr == '#' && *buff_len == 0) {
            while (getchar() != '\n') {
                if (feof(stdin))
                    return ERROR;
            }
            return BLANK;
        }
        if (isspace(chr)) {
            if (*buff_len == 0)
                return ERROR;
            else if ((*buffer)[*buff_len - 1] == ' ')
                continue;
            else
                (*buffer)[(*buff_len)++] = ' ';
        }
        else {
            (*buffer)[(*buff_len)++] = (char)chr;
        }
    }
}

bool tokenize_line(char *buffer, unsigned buff_len, char *command,
                   uint64_t *params, unsigned *params_num) {
    // Usuwa końcową spację, jeśli taka występuje.
    if (buff_len >= 2 && buffer[buff_len - 2] == ' ') {
        buffer[buff_len - 2] = '\0';
        --buff_len;
    }
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
        uint64_t parsed_num = strtoul(current_token, &next_token, 10);
        if (errno != 0 || current_token == next_token) {
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
                       gamma_golden_move(g, params[0],
                               params[1], params[2]));
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
                if (!board) {
                    line_error(line);
                }
                else {
                    printf("%s", board);
                    free(board);
                }
                return;
            }
        default:
            line_error(line);
    }
}

void batch_mainloop(unsigned line, gamma_t *g) {
    size_t buff_cap = 40;
    char *buffer = malloc(sizeof(char) * buff_cap);
    size_t buff_len;
    load_res load_result;
    char command;
    uint64_t params[4];
    unsigned params_num;

    while (!feof(stdin)) {
        load_result = load_line(&line, &buffer, &buff_cap, &buff_len);
        if (load_result == BLANK)
            continue;
        if (load_result == ERROR) {
            line_error(line);
            continue;
        }
        if (load_result == END) {
            free(buffer);
            return;
        }

        if (tokenize_line(buffer, buff_len, &command,
                          params, &params_num))
            interpret_statement(line, g, command, params, params_num);
        else
            line_error(line);
    }
    free(buffer);
}
