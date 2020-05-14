/** @file
 * Interfejs modułu udostępniającego grę gamma w trybie interaktywnym.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 5.05.2020
 */

#ifndef GAMMA_INTERACTIVE_MODE_H
#define GAMMA_INTERACTIVE_MODE_H

#include "gamma.h"

/** @brief Przeprowadza rozgrywkę w trybie interaktywnym.
 * Uruchamia tryb interaktywny rozgrywki w grę gamma w konsoli: czyści
 * zawartość ekranu, drukuje planszę zadaną w @p g, a następnie w pętli
 * umożliwia graczom wykonanie akcji: ruchu, złoty ruchu lub ominięcia
 * swojej kolejki, wyświetlając każdemu z nich potrzebne im statystyki gry.
 * Gdy żaden gracz nie może już wykonać ruchu lub po wciśnięciu kombinacji
 * klawiszy Ctrl-D, wypisuje podsumowanie zajętych pól przez każdego gracza
 * a następnie kończy działanie.
 * @param[in] g    - prawidłowa konfiguracja startowa gry gamma
 */
void interactive_game(gamma_t *g);



#endif //GAMMA_INTERACTIVE_MODE_H
