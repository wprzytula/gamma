/** @file
 * Moduł wykonywalny interfejsu tekstowego gry gamma.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 5.05.2020
 */

#include "batch_mode.h"
#include "interactive_mode.h"
#include "gamma.h"
#include <stdlib.h>

#define GAME_PARAMS_NUMBER 4        ///< Liczba parametrów gry.
#define INITIAL_BUFFER_CAPACITY 40  ///< Początkowa pojemność bufora.

/**
 * Tryb gry gamma.
 */
typedef enum {INVALID, BATCH, INTERACTIVE} game_mode;

/** @brief Określa tryb gry.
 * Określa, jaki tryb gry oznacza @p mode.
 * @param[in] mode  - znak określający tryb gry.
 * @return Wartość BATCH lub INTERACTIVE jeśli gra ma się odbyć w trybie
 * odpowiednio wsadowym lub interaktywnym, zaś INVALID jeśli podany znak
 * nie oznacza żadnego z powyższych trybów.
 */
static inline game_mode determine_mode(char mode) {
    if (mode == 'B')
        return BATCH;
    else if (mode == 'I')
        return INTERACTIVE;
    else
        return INVALID;
}

/** @brief Umożliwia wybór trybu rozgrywki oraz inicjuje ją.
 * Wczytuje ze standardowego wejścia dane na temat rozgrywki za pomocą
 * @ref load_line, parsuje je wraz z ich parametrami za pomocą
 * @ref tokenize_line oraz próbuje zainicjować nową rozgrywkę w danym trybie
 * z użyciem przekazanych parametrów. W przypadku sukcesu drukuje "OK @p line",
 * gdzie @p line jest numerem wiersza w którym wystąpiła prawidłowa inicjacja,
 * oraz uruchamia rozgrywkę w zadanym trybie. W przypadku niepowodzenia
 * woła @ref line_error z parametrem linii zawierającej błędne dane.
 * Ignoruje wiersze zawierające wyłącznie znak końca wiersza '\n', a także te
 * rozpoczynające się znakiem '#'. Kończy działanie wraz z końcem danych.
 * @return Wartość 0 jako kod pomyślnego zakończenia działania programu lub
 * inna wartość jeśli wystąpił krytyczny błąd.
 */
int main() {
    unsigned line = 0;
    size_t buff_cap = INITIAL_BUFFER_CAPACITY;
    char *buffer = malloc(sizeof(char) * buff_cap);
    size_t buff_len;
    char command;
    uint64_t params[GAME_PARAMS_NUMBER];
    unsigned params_num;
    load_res load_result;
    game_mode mode;
    gamma_t *g = NULL;
    do {
        load_result = load_line(&line, &buffer, &buff_cap, &buff_len);

        if (load_result == END) {
            free(buffer);
            return 0;
        }
        if (load_result == BLANK)
            continue;
        if (load_result == ERROR) {
            line_error(line);
            continue;
        }

        if (tokenize_line(buffer, buff_len, &command, params, &params_num)) {
            mode = determine_mode(command);
            if (mode != INVALID && params_num == GAME_PARAMS_NUMBER) {
                g = gamma_new(params[0], params[1],
                              params[2], params[3]);
                if (!g)
                    line_error(line);
            }
            else {
                line_error(line);
            }
        }
        else {
            line_error(line);
        }
    } while (!g);

    free(buffer);
    if (mode == BATCH) {
        printf("OK %u\n", line);
        batch_mainloop(line, g);
    }
    else {
        interactive_game(g);
    }
    gamma_delete(g);
    return 0;
}
