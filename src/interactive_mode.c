/** @file
 * Implementacja modułu udostępniającego grę gamma w trybie interaktywnym.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 5.05.2020
 */

#include "interactive_mode.h"
#include <ncurses.h>

void interactive_game(unsigned line, gamma_t *g) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    printw("Hello World !!!");	/* Print Hello World		  */
    refresh();			/* Print it on to the real screen */
    getch();			/* Wait for user input */
    getch();
    endwin();			/* End curses mode		  */
}
