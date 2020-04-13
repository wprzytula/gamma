/** @file
 * Implementacja klasy przechowującej stan gry gamma
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 25.03.2020
 */


#ifndef GAMMA_UTILS_H
#define GAMMA_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>


struct field {
    bool occupied;
    uint32_t occupied_by;
    bool already_visited;
    struct field *parent;
};


typedef struct field field_t;


typedef field_t* row_t;


typedef row_t* board_t;


typedef struct {
    bool golden_move_available;
    uint32_t occupied_areas;
    uint64_t taken_fields;
    uint64_t available_adjacent_fields;
} player_t;


struct gamma {
    uint32_t height;
    uint32_t width;
    uint32_t players_num;
    uint32_t max_areas;
    player_t *players;
    board_t board;
};


typedef struct gamma gamma_t;


typedef enum {TEST = 0, UNDO = 1, UNITE = 2} bfs_mode;


/** @brief Sprawdza, czy podane współrzędne są poprawne.
 * Sprawdza, czy pole (@p x, @p y) znajduje się na danej planszy,
 * czyli czy wartość współrzędnych @p x i @p y jest mniejsza
 * od wartości odpowiednio @p width i @p height z funkcji @ref gamma_new.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] x       – numer kolumny, liczba nieujemna
 * @param[in] y       – numer wiersza, liczba nieujemna
 * @return Wartość @p true, jeśli współrzędne są poprawne,
 * a @p false w przeciwnym przypadku.
 */
static inline bool valid_coords(gamma_t *g, uint32_t x, uint32_t y) {
    return (x < g->width && y < g->height);
}

/** @brief Sprawdza, czy podany gracz bierze udział w rozgrywce.
 * Sprawdza, czy wartość @p player jest mniejsza niż @p player
 * z funkcji @ref gamma_new.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] player  – numer gracza, liczba nieujemna
 * @return Wartość @p true, jeśli gracz bierze udział w rozgrywce,
 * a @p false w przeciwnym przypadku.
 */
static inline bool valid_player(gamma_t *g, uint32_t player) {
    return player < g->players_num;
}

/** @brief Daje wskaźnik na pole o danych współrzędnych.
* Zwraca referencję na pole (@p x, @p y).
* Zakłada, że @p g jest poprawną konfiguracją gry
 * oraz że pole (@p x, @p y) znajduje się na planszy zadanej przez @p g.
* @param[in,out] g  – wskaźnik na strukturę przechowującą stan gry
* @param[in] x      – numer kolumny, liczba nieujemna mniejsza od wartości
*                     @p width z funkcji @ref gamma_new,
* @param[in] y      – numer wiersza, liczba nieujemna mniejsza od wartości
*                     @p height z funkcji @ref gamma_new.
* @return Wskaźnik na pole (@p x, @p y).
*/
static inline field_t *get_field_ptr(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return &g->board[y][x];
}

/** @brief Sprawdza, czy podane pole jest zajęte.
 * Sprawdza, czy na polu (@p x, @p y) znajduje pionek dowolnego gracza.
 * Zakłada, że @p g jest poprawną konfiguracją gry oraz że pole
 * (@p x, @p y) znajduje się na planszy zadanej przez @p g.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli pole jest zajęte
 * a @p false w przeciwnym przypadku.
 */
static inline uint32_t is_occupied(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return g->board[y][x].occupied;
}

/** @brief Podaje właściciela pionka stojącego na danym polu.
 * Daje numer gracza, którego pionek znajduje się na polu (@p x, @p y).
 * Zakłada, że @p g jest poprawną konfiguracją gry, że pole (@p x, @p y)
 * znajduje się na planszy zadanej przez @p g oraz że to pole jest zajęte.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Numer gracza, którego pionek stoi na danym polu.
 */
static inline uint32_t get_owner(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    assert(is_occupied(g, x, y));
    return g->board[y][x].occupied_by;
}

/** @brief Ustawia podanego gracza jako właściciela pola.
 * Ustawia gracza @p player jako właściciela pola (@p x, @p y).
 * Zakłada, że @p g jest poprawną konfiguracją gry
 * oraz że pole (@p x, @p y) y znajduje się na planszy zadanej w @p g.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 */
static inline void set_owner(gamma_t *g, uint32_t player,
                             uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    g->board[y][x].occupied = true;
    g->board[y][x].occupied_by = player;
}

/** @brief Sprawdza, czy pole jest zajęte przez danego gracza.
 * Sprawdza, czy na polu (@p x, @p y) znajduje się pionek gracza @p player.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] player  - numer gracza, liczba nieujemna
 * @param[in] x       – numer kolumny, liczba nieujemna
 * @param[in] y       – numer wiersza, liczba nieujemna
 * @return Wartość @p true, jeśli pole (@p x, @p y) jest zajęte przez gracza
 * @p player, a @p false jeśli jest zajęte przez innego gracza,
 * jeśli jest pustelub jeśli nie znajduje się na planszy.
 */
static inline bool belongs_to_player(gamma_t *g, uint32_t player,
                                     uint32_t x, uint32_t y) {
    return valid_coords(g, x, y) && is_occupied(g, x, y)
           && get_owner(g, x, y) == player;
}

/** @brief Sprawdza, czy pole sąsiaduje z pionkiem danego gracza.
 * Sprawdza, czy na dowolnym z pól przyległych do pola (@p x, @p y)
 * znajduje się pionek gracza @p player.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] player  - numer gracza, liczba nieujemna
 * @param[in] x       – numer kolumny, liczba nieujemna
 * @param[in] y       – numer wiersza, liczba nieujemna
 * @return Wartość @p true, jeśli pole (@p x, @p y) sąsiaduje z pionkiem
 * gracza @p player, a @p false w przeciwnym przypadku.
 */
static inline bool has_neighbour_of_player(gamma_t *g, uint32_t player,
                             uint32_t x, uint32_t y) {
    return (belongs_to_player(g, player, x, y + 1)
            || belongs_to_player(g, player, x + 1, y)
            || belongs_to_player(g, player, x, y - 1)
            || belongs_to_player(g, player, x - 1, y));
}

/** @brief Ustawia pierwsze pole jako ojca drugiego pola.
 * Ustawia pole wskazywane przez @p parent_ptr jako ojca pola wskazywanego
 * przez @p child_ptr. Zakłada, że @p g jest poprawną konfiguracją gry,
 * a parent_ptr oraz child_ptr wskazują
 * na poprawne pola na planszy zadanej w @p g.
 * @param[in,out] g      – wskaźnik na strukturę przechowującą stan gry
 * @param[in] parent_ptr – wskaźnik na pole-ojca
 * @param[in] child_ptr  - wskaźnik na pole-syna
 */
static inline void set_parent(field_t *parent_ptr, field_t *child_ptr) {
    assert(parent_ptr != NULL && child_ptr != NULL);
    child_ptr->parent = parent_ptr;
}


static inline void compress_path(void *parent_ptr, void *child_ptr) {
    assert(parent_ptr != NULL && child_ptr != NULL);
    set_parent(parent_ptr, (field_t*)child_ptr);
}

/** @brief Daje reprezentanta obszaru, do którego należy pole.
 * Daje wskaźnik przodka pola wskazywanego przez @p filed_ptr, który
 * sam jest swoim ojcem. Zakłada, że field_ptr wskazuje
 * na poprawne pole na planszy zadanej w @p g.
 * @param[in,out] g      – wskaźnik na strukturę przechowującą stan gry
 * @param[in] field_ptr – wskaźnik na pole na planszy zadanej w @p g
 * @return Wskaźnik na reprezentanta obszaru do którego należy
 * pole wskazywane przez @p field_ptr.
 */
field_t* find_representative(field_t *field_ptr);

/** @brief Łączy obszary, do których należą dane dwa pola.
 * Ustawia reprezentanta obszaru, do którego należy pole (@p x1, @p y1),
 * jako reprezentanta obszaru do którego należy pole (@p x2, @p y2).
 * Zakłada, że @p g jest poprawną konfiguracją gry, oraz że (@p x1, @p y1)
 * i (@p x2, @p y2) są poprawnymi polami na planszy zadanej w @p g.
 * @param[in,out] g  – wskaźnik na strukturę przechowującą stan gry
 * @param[in] x1     – numer kolumny pola ojcowskiego, liczba nieujemna
 *                     mniejsza od wartości @p width z funkcji @ref gamma_new
 * @param[in] y1     - numer wiersza pola ojcowskiego, liczba nieujemna
 *                     mniejsza od wartości @p height z funkcji @ref gamma_new
 * @param[in] x2     – numer kolumny pola synowskiego, liczba nieujemna
 *                     mniejsza od wartości @p width z funkcji @ref gamma_new
 * @param[in] y2     – numer wiersza pola synowskiego, liczba nieujemna
 *                     mniejsza od wartości @p height z funkcji @ref gamma_new
 */
void unite_areas(gamma_t *g, uint32_t x1, uint32_t y1,
                 uint32_t x2, uint32_t y2);

/** @brief Sprawdza, czy pole zostało już odwiedzone
 *          przez algorytm przeszukujący.
 * Sprawdza, czy pole (@p x, @p y) zostało odwiedzone przez algorytm
 * przeszukujący w trybie TEST podczas wykonywania aktualnego złotego ruchu.
 * Zakłada, że @p g jest poprawną konfiguracją gry oraz że pole
 * (@p x, @p y) znajduje się na planszy zadanej w @p g.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli pole zostało już odwiedzone
 * a @p false w przeciwnym przypadku.
 */
static inline bool visited(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return g->board[y][x].already_visited;
}

/** @brief Ustawia flagę odwiedzin na danym polu na zadaną wartość.
 * Ustawia flagę @p visited pola (@p x, @p y) na wartość @p flag.
 * Zakłada, że @p g jest poprawną konfiguracją gry
 * oraz że pole (@p x, @p y) y znajduje się na planszy zadanej w @p g.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] flag    - wartość logiczna
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 */
static inline void set_visited(gamma_t *g, bool flag, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    g->board[y][x].already_visited = flag;
}


/** @brief Uruchamia na danym polu algorytm przeszukiwania obszaru danego gracza.
 * Przechodzi po całym obszarze w którego skład wchodzi pole (@p x, @p y)
 * w trybie @p mode, jeśli znajduje się na nim pionek gracza @p player.
 * ///TODO: opis trybów.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] player  - numer gracza, liczba nieujemna
 * @param[in] x       – numer kolumny, liczba nieujemna
 * @param[in] y       – numer wiersza, liczba nieujemna
 * @return W trybie TEST: wartość @p true, jeśli pole (@p x, @p y) należy do
 * gracza @p player oraz nie zostało jeszcze odwiedzone,
 * a @p false w przeciwnym przypadku. W trybach UNDO oraz UNITE:
 * wartość @p true, jeśli pole (@p x, @p y) należy do gracza @p player
 * oraz zostało już odwiedzone, a @p false w przeciwnym przypadku.
 */
bool start_bfs(gamma_t *g, bfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y);


int32_t calculate_new_owner_occ_areas_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]);


int32_t calculate_new_owner_adj_free_fields_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]);


int32_t calculate_former_owner_adj_free_fields_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]);


int32_t calculate_former_owner_occ_areas_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]);


#endif //GAMMA_UTILS_H
