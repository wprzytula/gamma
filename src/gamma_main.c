/** @file
 * Moduł wykonywalny gry gamma.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 5.05.2020
 */

#include "batch_mode.h"
#include "interactive_mode.h"
#include "gamma.h"


typedef enum {INVALID, BATCH, INTERACTIVE} game_mode;


static inline game_mode determine_mode(char command) {
    if (command == 'B')
        return BATCH;
    else if (command == 'I')
        return INTERACTIVE;
    else
        return INVALID;
}


void init_game(gamma_t **g, uint64_t params[], unsigned params_num) {
    if (params_num != 4)
        return;
    *g = gamma_new(params[0], params[1], params[2],params[3]);
}


int main() {
    unsigned line = 0;
    char buffer[100];
    unsigned buff_len;
    char command;
    uint64_t params[4];
    unsigned params_num;
    load_res load_result;
    game_mode mode;
    gamma_t *g = NULL;
    do {
        load_result = load_line(&line, buffer, &buff_len);
        if (load_result == BLANK)
            continue;
        if (load_result == ERROR) {
            line_error(line);
            continue;
        }
        if (load_result == END)
            return 0;

//        puts(buffer);

        tokenize_line(buffer, buff_len, &command, params, &params_num);

//        for (unsigned i = 0; i < params_num; ++i) {
//            printf("%lu\n", params[i]);
//        }

        mode = determine_mode(command);
        if (mode != INVALID && params_num == 4)
            init_game(&g, params, params_num);
        else
            line_error(line);
    } while (!g);
    if (mode == BATCH) {
        printf("OK %u\n", line);
        batch_mainloop(line, g);
    }
    else {// mode == INTERACTIVE
        interactive_game(g);
    }
    gamma_delete(g);
    return 0;
}