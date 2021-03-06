/** @file
 * Interfejs klasy przechowującej stan gry gamma.
 *
 * @author Marcin Peczarski <marpe@mimuw.edu.pl>
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 *
 * @copyright Uniwersytet Warszawski
 * @date 5.05.2020
 */

#ifndef GAMMA_H
#define GAMMA_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Typ przedstawiający stan gry.
 */
typedef struct gamma gamma_t;

/** @brief Tworzy strukturę przechowującą stan gry.
 * Alokuje pamięć na nową strukturę przechowującą stan gry.
 * Inicjuje tę strukturę tak, aby reprezentowała początkowy stan gry.
 * @param[in] width   - szerokość planszy, liczba dodatnia,
 * @param[in] height  - wysokość planszy, liczba dodatnia,
 * @param[in] players - liczba graczy, liczba dodatnia,
 * @param[in] areas   - maksymalna liczba obszarów,
 *                      jakie może zająć jeden gracz, liczba dodatnia.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci lub któryś z parametrów jest niepoprawny.
 */
gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas);

/** @brief Usuwa strukturę przechowującą stan gry.
 * Usuwa z pamięci strukturę wskazywaną przez @p g.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] g       - wskaźnik na usuwaną strukturę.
 */
void gamma_delete(gamma_t *g);

/** @brief Daje długość napisu najwyższego numeru gracza biorącego udział
 * w rozgrywce.
 * @param[in] g   - wskaźnik na strukturę przechowującą stan gry.
 * @return Liczba cyfr w napisie najwyższego numeru gracza biorącego udział
 * w rozgrywce, lub 0 gdy @p g jest NULL.
 */
unsigned gamma_get_max_player_len(gamma_t *g);

/** @brief Daje liczbę graczy biorących udział w rozgrywce.
 * @param[in] g   - wskaźnik na strukturę przechowującą stan gry.
 * @return Liczba graczy biorących udział w rozgrywce,
 * lub 0 gdy @p g jest NULL.
 */
uint32_t gamma_get_players_num(gamma_t *g);

/** @brief Daje szerokość planszy w zadanej rozgrywce.
 * @param[in] g   - wskaźnik na strukturę przechowującą stan gry.
 * @return Szerokość planszy w zadanej rozgrywce,
 * lub 0 gdy @p g jest NULL.
 */
uint32_t gamma_get_width(gamma_t *g);

/** @brief Daje wysokość planszy w zadanej rozgrywce.
 * @param[in] g   - wskaźnik na strukturę przechowującą stan gry.
 * @return Wysokość planszy w zadanej rozgrywce,
 * lub 0 gdy @p g jest NULL.
 */
uint32_t gamma_get_height(gamma_t *g);

/** @brief Daje informację o graczu zajmującym pole.
 * Daje numer gracza, którego pionek zajmuje pole (@p x, @p y).
 * @param[in] g   - wskaźnik na strukturę przechowującą stan gry.
 * @param[in] x   - numer kolumny, liczba nieujemna mniejsza od wartości
 *                  @p width z funkcji @ref gamma_new,
 * @param[in] y   - numer wiersza, liczba nieujemna mniejsza od wartości
 *                  @p height z funkcji @ref gamma_new.
 * @return Numer gracza, którego pionek stoi na zadanym polu lub 0,
 * jeśli pole jest puste.
 */
uint32_t gamma_get_owner(gamma_t *g, uint32_t x, uint32_t y);

/** @brief Podaje liczbę pól zajętych przez gracza.
 * Podaje liczbę pól zajętych przez gracza @p player.
 * @param[in] g
 * - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  - numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól zajętych przez gracza lub zero,
 * jeśli któryś z parametrów jest niepoprawny.
 */
uint64_t gamma_busy_fields(gamma_t *g, uint32_t player);

/** @brief Podaje liczbę pól, jakie jeszcze gracz może zająć.
 * Podaje liczbę wolnych pól, na których w danym stanie gry gracz @p player może
 * postawić swój pionek w następnym ruchu.
 * @param[in] g       - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  - numer gracza, liczba dodatnia niewiększa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Liczba pól, jakie jeszcze może zająć gracz lub zero,
 * jeśli któryś z parametrów jest niepoprawny.
 */
uint64_t gamma_free_fields(gamma_t *g, uint32_t player);

/** @brief Określa, czy na dane pole może zostać wykonany ruch.
 * Sprawdza, czy gracz @p player może wykonać ruch na pole (@p x, @p y)
 * tak, by w efekcie nie zostały przekroczone limity liczby obszarów na gracza.
 * @param[in] g         - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player    - numer gracza mającego wykonać ruch,
 *                        liczba dodatnia nie większa od wartości
 *                        @p players z funkcji @ref gamma_new,
 * @param[in] x         - numer kolumny, liczba nieujemna,
 * @param[in] y         - numer wiersza, liczba nieujemna.
 * @return Wartość @p true, jeśli gracz @p player może wykonać ruch
 * na pole (@p x, @p y), lub @p false w przeciwnym przypadku.
 */
bool gamma_move_possible_there(gamma_t *g, uint32_t player, uint32_t x,
                               uint32_t y);

/** @brief Określa, czy gracz może wykonać złoty ruch na dane pole.
 * Sprawdza, czy gracz @p attacker może wykonać złoty ruch na pole (@p x, @p y)
 * tak, by w efekcie nie zostały przekroczone limity liczby obszarów na gracza.
 * @param[in] g         - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] attacker  - numer gracza atakującego w złotym ruchu,
 *                        liczba dodatnia nie większa od wartości
 *                        @p players z funkcji @ref gamma_new,
 * @param[in] x         - numer kolumny, liczba nieujemna,
 * @param[in] y         - numer wiersza, liczba nieujemna.
 * @return Wartość @p true, jeśli gracz @p attacker może wykonać złoty ruch
 * na pole (@p x, @p y), lub @p false w przeciwnym przypadku.
 */
bool gamma_golden_possible_there(gamma_t *g, uint32_t attacker,
                                 uint32_t x, uint32_t y);

/** @brief Sprawdza, czy gracz może wykonać złoty ruch.
 * Sprawdza, czy gracz @p player jeszcze nie wykonał w tej rozgrywce złotego
 * ruchu i jest przynajmniej jedno pole zajęte przez innego gracza, na które
 * dozwolone jest wykonanie złotego ruchu przez gracza @p player w tej kolejce.
 * @param[in] g       - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  - numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli gracz jeszcze nie wykonał w tej rozgrywce
 * złotego ruchu i jest przynajmniej jedno pole zajęte przez innego gracza,
 * a @p false w przeciwnym przypadku.
 */
bool gamma_golden_possible(gamma_t *g, uint32_t player);

/** @brief Wykonuje ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y).
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  - numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       - numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       - numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli ruch został wykonany, a @p false,
 * gdy ruch jest nielegalny lub któryś z parametrów jest niepoprawny.
 */
bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Wykonuje złoty ruch.
 * Ustawia pionek gracza @p player na polu (@p x, @p y) zajętym przez innego
 * gracza, usuwając pionek innego gracza.
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  - numer gracza, liczba dodatnia nie większa od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       - numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       - numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli ruch został wykonany, a @p false,
 * gdy gracz wykorzystał już swój złoty ruch, ruch jest nielegalny
 * lub któryś z parametrów jest niepoprawny.
 */
bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);

/** @brief Daje napis opisujący stan planszy.
 * Alokuje w pamięci bufor, w którym umieszcza napis zawierający tekstowy
 * opis aktualnego stanu planszy. Przykład znajduje się w pliku gamma_test.c.
 * Funkcja wywołująca musi zwolnić ten bufor.
 * @param[in] g       - wskaźnik na strukturę przechowującą stan gry.
 * @return Wskaźnik na zaalokowany bufor zawierający napis opisujący stan
 * planszy lub NULL, jeśli nie udało się zaalokować pamięci.
 */
char* gamma_board(gamma_t *g);

#endif /* GAMMA_H */
