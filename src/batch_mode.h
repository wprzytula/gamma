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

/**
 * Typ wyniku ładowania linii wejścia.
 */
typedef enum {END, BLANK, ERROR, VALID} load_res;

/** @brief Ładuje do bufora pojedynczą linię ze standardowego wejścia.
 * Ładuje znaki do bufora aż napotka znak końca linii lub koniec danych.
 * Jeśli linia jest poprawna i znacząca, czyli zawiera co najmniej dwa znaki
 * i kończy się znakiem '\n', wówczas @p buffer zawiera wczytane dane
 * i zwracana jest wartość VALID.
 * @param[in,out] line      - numer poprzedniej linii,
 * @param[out] buffer       - bufor, do którego ładowane do znaki z wejścia,
 * @param[in, out] buff_cap - pojemność bufora,
 * @param[out] buff_len     - długość danych w buforze.
 * @return Wartość END, jeśli linia jest pusta (dotarto do końca wejścia);
 * BLANK, jeśli linia zaczyna się znakiem '#' lub zawiera tylko znak '\n';
 * ERROR, jeśli nastąpiło przepełnienie bufora, lub plik kończy się niepustą
 * linią niezakończoną znakiem '\n'; w pozostałych przypadkach VALID.
 */
load_res load_line(unsigned *line, char **buffer, size_t *buff_cap,
                   size_t *buff_len);

/** @brief Drukuje informację o błędnej linii.
 * Drukuje na standardowe wyjście diagnostyczne komunikat o błędzie
 * wraz z numerem linii w której wystąpił.
 * @param[in] line  -  numer linii w której podano błędne polecenie,
 *                     liczba nieujemna.
 */
static inline void line_error(unsigned line) {
    fprintf(stderr, "ERROR %u\n", line);
}

/** @brief Parsuje polecenie wraz parametrami.
 * Parsuje polecenie, korzystając z pojedynczych spacji jako separatorów.
 * Zapisuje pierwszy znak w @p command oraz kolejne tokeny odczytane jako
 * liczby typu  @a uint64_t w tablicy @p params. Dopuszcza sytuację
 * że na końcu danych znajduje się pojedyncza spacja.
 * @param[in] buffer       - bufor zawierający znaki z wejścia,
 * @param[in] buff_len     - długość danych w buforze,
 * @param[out] command     - odczytany znak polecenia,
 * @param[out] params      - tablica sparsowanych parametrów polecenia,
 * @param[out] params_num  - liczba sparsowanych parametrów polecenia.
 * @return Wartość @a true, jeśli dane zostały sparsowane poprawnie, czyli
 * jeśli pierwszy znak danych jest literą, drugi jest spacją, parametry są
 * liczbami mieszczącymi się w zakresie @p uint64_t i jest ich co najwyżej 4;
 * w przeciwnym przypadku zwraca @a false.
 */
bool tokenize_line(char *buffer, unsigned buff_len, char *command,
                   uint64_t *params, unsigned *params_num);

/** @brief Wykonuje polecenie wraz z parametrami i drukuje jego wynik.
 * Wykonuje polecenie określone przez znak @p command wraz z parametrami
 * @p params oraz wypisuje zwrócony wynik na standardowe wyjście.
 * Jeśli nie istnieje polecenie określone znakiem @p command lub liczba podanych
 * parametrów jest niezgodna z oczekiwaną przez określone polecenie, wówczas
 * wywoływana jest funkcja @p line_error wraz z parametrem @p line.
 * @param[in] line         - numer linii z wykonywanym poleceniem,
 * @param[in,out] g        - prawidłowa konfiguracja gry gamma,
 * @param[in] command      - znak wykonywanego polecenia,
 * @param[in] params       - tablica sparsowanych parametrów polecenia,
 * @param[in] params_num   - liczba sparsowanych parametrów polecenia.
 */
void interpret_statement(unsigned line, gamma_t *g, char command,
                         uint64_t *params, unsigned params_num);

/** @brief Wykonuje w pętli przekazywane wraz z parametrami polecenia.
 * Wczytuje ze standardowego wejścia polecenia za pomocą @ref load_line,
 * parsuje je wraz z ich parametrami za pomocą @ref tokenize_line oraz wykonuje
 * je za pomocą @ref interpret_statement. W przypadku pomyślnego wykonania
 * polecenia drukuje jego wynik, zaś w przypadku błędu woła @ref line_error
 * z parametrem linii zawierającej błędne dane. Ignoruje wiersze zawierające
 * wyłącznie znak końca wiersza '\n' oraz rozpoczynające się znakiem '#'.
 * Kończy działanie wraz z końcem danych.
 * @param[in] line    - numer linii, w której  rozpoczęto działanie
 *                      trybu wsadowego,
 * @param[in] g   - prawidłowa konfiguracja startowa gry gamma.
 */
void batch_mainloop(unsigned line, gamma_t *g);

#endif //GAMMA_BATCH_MODE_H
