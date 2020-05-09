/** @file
 * Implementacja modułu udostępniającego grę gamma w trybie interaktywnym.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 5.05.2020
 */

#include "interactive_mode.h"
#include "stdlib.h"
#include "stdio.h"
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

#define ANSI "\x1b["
#define ASCII_EOF 4
#define ESC 27

typedef enum {LEFT = 'D', UP = 'A', DOWN = 'B', RIGHT = 'C'} arrow;

static void update_board(gamma_t *g, uint32_t x, uint32_t y, uint32_t player) {
    printf(ANSI"s");

    printf(ANSI"%u;%uH", gamma_get_height(g) - y, x + 1);
    printf("%u", player);

    printf(ANSI"u");
}
static void show_summary(gamma_t *g) {
    printf(ANSI"%u;%uH", gamma_get_height(g) + 1, 0);
    printf(ANSI"K");
    uint64_t max_busy_fields = 0;
    for (uint32_t player = 1; player <= gamma_get_players_num(g)
                              && player > 0; ++player ) {
        if (gamma_busy_fields(g, player) > max_busy_fields)
            max_busy_fields = gamma_busy_fields(g, player);
    }
    for (uint32_t player = 1; player <= gamma_get_players_num(g)
            && player > 0; ++player ) {
        printf("PLAYER %u HAS TAKEN: %lu%s\n", player,
           gamma_busy_fields(g, player),
           (gamma_busy_fields(g, player) == max_busy_fields) ? " - WINS!" : "");
    }
}

static void update_stats(gamma_t *g, uint32_t curr_player) {
    printf(ANSI"s");
    printf(ANSI"%u;%uH", gamma_get_height(g) + 1, 0);
    printf(ANSI"K");
    printf("PLAYER %u B%lu F%lu%s", curr_player,
            gamma_busy_fields(g, curr_player),
            gamma_free_fields(g, curr_player),
            gamma_golden_possible(g, curr_player) ? " G" : "");

    printf(ANSI"u");
}

static void move_cursor(gamma_t *g, uint32_t *x, uint32_t *y, arrow dirchr) {
    switch (dirchr) {
        case UP:
            if (*y < gamma_get_height(g) - 1) {
                printf(ANSI"1A");
                ++*y;
            }
            break;

        case DOWN:
            if (*y > 0) {
                printf(ANSI"1B");
                --*y;
            }
            break;
        case RIGHT:
            if (*x < gamma_get_width(g) - 1) {
                printf(ANSI"1C");
                ++*x;
            }
            break;
        case LEFT:
            if (*x > 0) {
                printf(ANSI"1D");
                --*x;
            }
            break;
        default:
            break;
    }
}

static void whereami(gamma_t *g, uint32_t x, uint32_t y) {
    printf(ANSI"s");
    printf(ANSI"%u;%uH", 0, gamma_get_width(g) + 4);
    printf(ANSI"K");
    printf("%u, %u", x, y);

    printf(ANSI"u");
}

static bool player_turn(gamma_t *g, uint32_t player, uint32_t *x, uint32_t *y) {
    int chr = -1;
    int lastchr = -1;
    int beforelastchr;
    while (true) {
//        whereami(g, *x, *y);
        beforelastchr = lastchr;
        lastchr = chr;
        chr = getchar();
        switch (chr) {
            case ' ':
                if (gamma_move(g, player, *x, *y)) {
                    update_board(g, *x, *y, player);
                    return true;
                }
                break;
            case 'g':
            case 'G':
                if (gamma_golden_move(g, player, *x, *y)) {
                    update_board(g, *x, *y, player);
                    return true;
                }
                break;

            case 'A':
                if (beforelastchr == ESC && lastchr == '[') {
                    move_cursor(g, x, y, UP);
                }
                break;
            case 'B':
                if (beforelastchr == ESC && lastchr == '[') {
                    move_cursor(g, x, y, DOWN);
                }
                break;
            case 'D':
                if (beforelastchr == ESC && lastchr == '[') {
                    move_cursor(g, x, y, LEFT);
                }
                break;
            case 'C':
                if (beforelastchr == ESC && lastchr == '[')
                    move_cursor(g, x, y, RIGHT);
                else
                    return true;
                break;
            case 'c':
                return true;

            case ASCII_EOF:
                return false;

            default:
                break;
        }
    }
}

void interactive_game(gamma_t *g) {
    uint32_t x = 0;
    uint32_t y = gamma_get_height(g) - 1;

    printf(ANSI"2J");

    struct termios normal_term, inter_term;

    tcgetattr(STDIN_FILENO, &normal_term);
    inter_term = normal_term;

    inter_term.c_lflag &= ~(ICANON);
    //inter_term.c_lflag &= ~(IUCLC);
    inter_term.c_lflag &= ~(ICANON | ECHO);


    tcsetattr( STDIN_FILENO, TCSANOW, &inter_term);

    printf(ANSI"0;0H");
    char *board = gamma_board(g);
    printf("%s", board);
    free(board);
    printf(ANSI"0;0H");

    uint32_t curr_player = 1;
    uint32_t last_able = 0;
    while (true) {
//        fprintf(stderr, "Last_able %u, current %u\n", last_able, curr_player);
        if (gamma_golden_possible(g, curr_player)
            || gamma_free_fields(g, curr_player) != 0) {
            last_able = curr_player;
            update_stats(g, curr_player);
            if (!player_turn(g, curr_player, &x, &y))
                break;
        }
        else if (last_able == curr_player)
            break;

        if (curr_player < gamma_get_players_num(g))
            ++curr_player;
        else
            curr_player = 1;
    }
    show_summary(g);
    getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &normal_term);

    // clean up
    printf(ANSI"2J");
    printf(ANSI"0;0H");
}

