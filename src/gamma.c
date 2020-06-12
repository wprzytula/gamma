/** @file
 * Implementacja klasy przechowującej stan gry gamma.
 * Eksportowane funkcje (gamma_*) przyjmują dodatni numer gracza, a wewnętrznie
 * przetwarzają go jako mniejszy o 1.
 * Funkcje statyczne przyjmują nieujemny numer gracza, tj. numer o 1 mniejszy
 * od faktycznego w grze.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 25.03.2020
 */

#include "gamma.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>

#define DIRECTIONS 4   ///< Liczba kierunków położenia przyległych pól
                       ///< względem siebie.

/**
 * Typ przedstawiający położenie pól względem siebie.
 */
typedef enum {UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3, RET = 4} direction_t;

/**
 * Struktura przechowująca stan pola na planszy.
 * Implementuje także strukturę Find & Union na planszy. Działa ona następująco:
 * Każde pole należące do spójnego obszaru gracza ma przydzielonego ojca,
 * czyli pewne pole należące również do tego obszaru. Któryś z kolei ojciec
 * sam jest swoim ojcem - nazywa się go przodkiem. Do jego znalezienia służy
 * @ref find_representative. Aby zapobiec tworzeniu się zbyt długich łańcuchów
 * prowadzących do przodka, każde wywołanie @ref find_representative dokonuje
 * kompresji ścieżek, czyli ustawia przodka jako bezpośredniego ojca pól
 * (maksymalnie pierwszych dziesięciu), po których nastąpiło do niego dojście.
 * Łączenie obszarów zapewnia @ref unite_areas, zaś dzielenie jest możliwe
 * tylko przez utworzenie ich od nowa poprzez użycie algorytmu przeszukującego
 * @ref start_dfs.
 */
struct field {
    bool occupied;          ///< Czy na polu stoi pionek.
    uint32_t occupied_by;   ///< Czyj pionek stoi na polu.
    bool visited;           ///< Czy już odwiedzone przez algorytm przeszukujący.
    direction_t base_dir;   ///< Skąd pierwotnie pojawił się na tym polu
                            ///< algorytm przeszukujący.
    struct field *parent;   ///< Wskaźnik na pole-ojca w tym samym obszarze.
};

/**
 * Typ przedstawiający pole na planszy.
 */
typedef struct field field_t;

/**
 * Typ przedstawiający rząd pól na planszy.
 */
typedef field_t* row_t;

/**
 * Typ przedstawiający planszę.
 */
typedef row_t* board_t;

/**
 * Typ struktury przechowującej dane na temat gracza.
 */
typedef struct {
    bool golden_move_used;         ///< Czy gracz nie wykonał już złoty ruch.
    uint32_t occupied_areas;       ///< Ile obszarów zajmuje gracz na planszy.
    uint64_t taken_fields;         ///< Ile pól zajmuje gracz na planszy.
    uint64_t available_adj_fields; ///< Ile pustych pól przylega do pionków gracza.
} player_t;

/**
 * Struktura przechowująca stan gry.
 */
struct gamma {
    uint32_t height;          ///< Wysokość planszy.
    uint32_t width;           ///< Szerokość planszy.
    uint32_t players_num;     ///< Liczba graczy biorących udział w rozgrywce.
    uint32_t max_areas;       ///< Maksymalna dopuszczalna liczba obszarów
                              ///< dla każdego gracza.
    unsigned players_num_len; ///< Długość napisu gracza o najwyższym numerze.
    uint64_t free_fields;     ///< Liczba pustych pól na planszy.
    player_t *players;        ///< Tablica indywidualnych konfiguracji graczy,
                              ///< przechowuje gracza z numerem @p n
                              ///< w miejscu o indeksie @p n-1.
    board_t board;            ///< Plansza do gry.
};

/**
 * Typ trybu działania algorytmu przeszukującego obszary zaatakowanego
 * w złotym ruchu gracza sąsiadujące z zaatakowanym polem.
 */
typedef enum {TEST, UNDO, UNITE} dfs_mode;

/** @brief Sprawdza, czy podane współrzędne są poprawne.
 * Sprawdza, czy pole (@p x, @p y) znajduje się na danej planszy,
 * czyli czy wartość współrzędnych @p x i @p y jest mniejsza
 * od wartości odpowiednio @p width i @p height z funkcji @ref gamma_new.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry
 * @param[in] x       - numer kolumny, liczba nieujemna
 * @param[in] y       - numer wiersza, liczba nieujemna
 * @return Wartość @p true, jeśli współrzędne są poprawne,
 * a @p false w przeciwnym przypadku.
 */
static inline bool valid_coords(gamma_t *g, uint32_t x, uint32_t y) {
    return (x < g->width && y < g->height);
}

/** @brief Sprawdza, czy podany gracz bierze udział w rozgrywce.
 * Sprawdza, czy wartość @p player jest mniejsza niż @p player
 * z funkcji @ref gamma_new.
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry
 * @param[in] player  - numer gracza, liczba nieujemna
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
* @param[in,out] g  - wskaźnik na strukturę przechowującą stan gry
* @param[in] x      - numer kolumny, liczba nieujemna mniejsza od wartości
*                     @p width z funkcji @ref gamma_new,
* @param[in] y      - numer wiersza, liczba nieujemna mniejsza od wartości
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
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry
 * @param[in] x       - numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       - numer wiersza, liczba nieujemna mniejsza od wartości
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
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry
 * @param[in] x       - numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       - numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Numer gracza, którego pionek stoi na danym polu.
 */
static inline uint32_t get_owner(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    assert(is_occupied(g, x, y));
    return g->board[y][x].occupied_by;
}

/** @brief Ustawia podanego gracza jako zajmującego pole.
 * Ustawia gracza @p player jako zajmującego pole (@p x, @p y).
 * Zakłada, że @p g jest poprawną konfiguracją gry, że gracz @p player
 * bierze udział w rozgrywce oraz że pole (@p x, @p y) y
 * znajduje się na planszy zadanej w @p g.
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  - numer gracza, liczba nieujemna mniejsza od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       - numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       - numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 */
static inline void set_owner(gamma_t *g, uint32_t player,
                             uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    assert(valid_player(g, player));
    g->board[y][x].occupied = true;
    g->board[y][x].occupied_by = player;
}

/** @brief Sprawdza, czy pole jest zajęte przez danego gracza.
 * Sprawdza, czy na polu (@p x, @p y) znajduje się pionek gracza @p player.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry
 * @param[in] player  - numer gracza, liczba nieujemna
 * @param[in] x       - numer kolumny, liczba nieujemna
 * @param[in] y       - numer wiersza, liczba nieujemna
 * @return Wartość @p true, jeśli pole (@p x, @p y) jest zajęte przez gracza
 * @p player, a @p false jeśli jest zajęte przez innego gracza,
 * jeśli jest puste lub jeśli nie znajduje się na planszy.
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
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry
 * @param[in] player  - numer gracza, liczba nieujemna
 * @param[in] x       - numer kolumny, liczba nieujemna
 * @param[in] y       - numer wiersza, liczba nieujemna
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
 * przez @p child_ptr. Zakłada, że parent_ptr oraz child_ptr wskazują
 * na poprawne pola na planszy zadanej w @ref gamma_new.
 * @param[in] parent_ptr    - wskaźnik na pole-ojca
 * @param[in,out] child_ptr - wskaźnik na pole-syna
 */
static inline void set_parent(field_t *parent_ptr, field_t *child_ptr) {
    assert(parent_ptr != NULL && child_ptr != NULL);
    child_ptr->parent = parent_ptr;
}

/** @brief Daje reprezentanta obszaru, do którego należy pole.
 * Daje wskaźnik przodka pola wskazywanego przez @p field_ptr, który
 * sam jest swoim ojcem. Dodatkowo dokonuje kompresji ścieżki, ustawiając
 * znalezionego przodka jako ojca wszystkich swoich potomków. Zakłada, że
 * field_ptr wskazuje na poprawne pole na planszy zadanej w @ref gamma_new.
 * @param[in,out] field_ptr - wskaźnik na pole na planszy zadanej
 *                              w @ref gamma_new.
 * @return Wskaźnik na reprezentanta obszaru do którego należy
 * pole wskazywane przez @p field_ptr.
 */
static field_t *find_representative(field_t *field_ptr) {
    assert(field_ptr);
    field_t *trace[10];
    unsigned quantity = 0;

    while (field_ptr->parent != field_ptr) {
        assert(field_ptr);
        if (quantity < 10)
            trace[quantity++] = field_ptr;
        field_ptr = field_ptr->parent;
    }
    for (unsigned i = 0; i < quantity; ++i) {
        set_parent(field_ptr, trace[i]);
    }
    return field_ptr;
}

/** @brief Łączy obszary, do których należą dane dwa pola.
 * Ustawia reprezentanta obszaru, do którego należy pole (@p x1, @p y1),
 * jako reprezentanta obszaru do którego należy pole (@p x2, @p y2).
 * Zakłada, że @p g jest poprawną konfiguracją gry oraz że (@p x1, @p y1)
 * i (@p x2, @p y2) są poprawnymi polami na planszy zadanej w @p g.
 * @param[in,out] g  - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x1     - numer kolumny pola ojcowskiego, liczba nieujemna
 *                     mniejsza od wartości @p width z funkcji @ref gamma_new,
 * @param[in] y1     - numer wiersza pola ojcowskiego, liczba nieujemna
 *                     mniejsza od wartości @p height z funkcji @ref gamma_new,
 * @param[in] x2     - numer kolumny pola synowskiego, liczba nieujemna
 *                     mniejsza od wartości @p width z funkcji @ref gamma_new,
 * @param[in] y2     - numer wiersza pola synowskiego, liczba nieujemna
 *                     mniejsza od wartości @p height z funkcji @ref gamma_new.
 */
static void unite_areas(gamma_t *g, uint32_t x1, uint32_t y1,
                 uint32_t x2, uint32_t y2) {
    field_t *representative1 = find_representative(get_field_ptr(g, x1, y1));
    field_t *representative2 = find_representative(get_field_ptr(g, x2, y2));

    if (representative1 != representative2)
        set_parent(representative1, representative2);
}

/** @brief Sprawdza, czy pole zostało już odwiedzone przez algorytm przeszukujący.
 * Sprawdza, czy pole (@p x, @p y) zostało odwiedzone przez algorytm
 * przeszukujący w trybie @p mode podczas wykonywania aktualnego złotego ruchu.
 * Zakłada, że @p g jest poprawną konfiguracją gry oraz że pole
 * (@p x, @p y) znajduje się na planszy zadanej w @p g.
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] mode    - tryb algorytmu przeszukującego,
 * @param[in] x       - numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       - numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 * @return Wartość @p true, jeśli pole zostało już odwiedzone przez algorytm
 * przeszukujący w trybie @p mode, a @p false w przeciwnym przypadku.
 */
static inline bool was_visited(gamma_t *g, dfs_mode mode,
                               uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return mode == TEST ? g->board[y][x].visited : !g->board[y][x].visited;
}

/** @brief Ustawia flagę odwiedzin na danym polu na zadaną wartość.
 * Ustawia flagę @p visited pola (@p x, @p y) na wartość @p flag.
 * Zakłada, że @p g je  st poprawną konfiguracją gry
 * oraz że pole (@p x, @p y) y znajduje się na planszy zadanej w @p g.
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] flag    - wartość logiczna,
 * @param[in] x       - numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       - numer wiersza, liczba nieujemna mniejsza od wartości
 *                      @p height z funkcji @ref gamma_new.
 */
static inline void set_visited(gamma_t *g, bool flag, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    g->board[y][x].visited = flag;
}

/** @brief Uruchamia na danym polu algorytm przeszukiwania obszaru danego gracza.
 * Przechodzi po całym obszarze w którego skład wchodzi pole (@p x, @p y)
 * w trybie @p mode, jeśli znajduje się na nim pionek gracza @p player.
 * W trybie @p TEST przechodzi po polach z flagą @p visited ustawioną na false,
 * zaś w trybach @p UNDO i @p UNITE po polach flagą @p visited ustawioną na true.
 * W trybie UNITE dodatkowo łączy odwiedzane pola w jeden spójny obszar.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g   - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] mode    - tryb pracy algorytmu przeszukującego, jedna
 *                       wartości typu @ref dfs_mode,
 * @param[in] player  - numer gracza, liczba nieujemna,
 * @param[in] x       - numer kolumny, liczba nieujemna,
 * @param[in] y       - numer wiersza, liczba nieujemna.
 * @return W trybie @p TEST: wartość @p true, jeśli pole (@p x, @p y) należy do
 * gracza @p player oraz nie zostało jeszcze odwiedzone,
 * a @p false w przeciwnym przypadku. W trybach @p UNDO oraz @p UNITE:
 * wartość @p true, jeśli pole (@p x, @p y) należy do gracza @p player
 * oraz zostało już odwiedzone, a @p false w pozostałych przypadkach.
 */
static bool start_dfs(gamma_t *g, dfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y) {
    assert(valid_player(g, player));
    if (!belongs_to_player(g, player, x, y))
        return false;
    if (was_visited(g, mode, x, y))
        return false;

    field_t *new_representative = NULL;

    if (mode == UNITE) {
        new_representative = get_field_ptr(g, x, y);
        set_parent(new_representative, new_representative);
    }

    uint32_t x_coords[DIRECTIONS];
    uint32_t y_coords[DIRECTIONS];

    g->board[y][x].base_dir = RET;
    bool ret = true;
    direction_t direction = UP;
    uint32_t prev_x;
    set_visited(g, mode == TEST, x, y);

    do {
        x_coords[UP]    = x     ; y_coords[UP]    = y + 1;
        x_coords[RIGHT] = x + 1 ; y_coords[RIGHT] = y    ;
        x_coords[DOWN]  = x     ; y_coords[DOWN]  = y - 1;
        x_coords[LEFT]  = x - 1 ; y_coords[LEFT]  = y    ;

        if (!belongs_to_player(g, player, x, y) && ret)
            assert(false);
        if (!belongs_to_player(g, player, x, y)
            || (was_visited(g, mode, x, y) && !ret)) {
            x = x_coords[(direction + 2) % DIRECTIONS];
            y = y_coords[(direction + 2) % DIRECTIONS];
            ret = true;
            ++direction;
        }
        else if (!was_visited(g, mode, x, y) && ret) {
            assert(false);
        }
        else if (!ret) {
            // Przygotowanie pola do cyklu.
            set_visited(g, mode == TEST, x, y);
            g->board[y][x].base_dir = (direction + 2) % DIRECTIONS;
            if (mode == UNITE)
                set_parent(new_representative, get_field_ptr(g, x, y));
            direction = UP;
            ret = true;
        }
        else {
            if (direction == RET) {
                direction = (g->board[y][x].base_dir + 2) % DIRECTIONS + 1;
                prev_x = x;
                x = x_coords[g->board[y][x].base_dir];
                y = y_coords[g->board[y][prev_x].base_dir];

            }
            else if (direction == g->board[y][x].base_dir) {
                ++direction;
            }
            else {
                x = x_coords[direction];
                y = y_coords[direction];
                ret = false;
            }
        }
    } while (!valid_coords(g, x, y) || direction < RET
             || g->board[y][x].base_dir < RET );
    return true;
}

/** @brief Oblicza zmianę liczby pustych pól przyległych do obszarów gracza
 * atakującego lub atakowanego w złotym ruchu.
 * Zlicza pola (@p x_coords[i], @p y_coords[i]) dla 0 <= i < 4, jeśli przed
 * złotym ruchem nie sąsiadują z żadnym polem zajętym przez gracza @p player.
 * Jeśli wartością @p count_for_new_owner jest @p true, wówczas zwracany jest
 * nieujemny wynik, a w przeciwnym przypadku zwracana jest liczba przeciwna
 * do tego nieujemnego wyniku. Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g               - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] count_for_new_owner - czy obliczenia dotyczą gracza atakującego
 *                                  (w przeciwnym przypadku - gracza atakowanego)
 * @param[in] player              - numer gracza atakowanego lub atakującego
 *                                  w złotym ruchu, liczba nieujemna,
 * @param[in] x_coords            - tablica numerów kolumn pól przyległych
 *                                  do pola na które wykonywany jest złoty ruch,
 *                                  liczba nieujemna,
 * @param[in] y_coords            - tablica numerów wierszy pól przyległych
 *                                  do pola na które wykonywany jest złoty ruch,
 *                                  liczba nieujemna.
 * @return Liczba, o jaką zmieniła się liczba pustych pól przyległych
 * do obszarów gracza @p player.
 */
static int calculate_player_adj_free_fields_change(
        gamma_t *g, bool count_for_new_owner, uint32_t player,
        uint32_t *x_coords, uint32_t *y_coords) {
    int player_adj_fields_change = 0;
    uint32_t x_;
    uint32_t y_;

    for (unsigned i = 0; i < DIRECTIONS; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (valid_coords(g, x_, y_) && !is_occupied(g, x_, y_)
            && !has_neighbour_of_player(g, player, x_, y_))
            ++player_adj_fields_change;
    }
    return count_for_new_owner ?
            player_adj_fields_change : -player_adj_fields_change;
}

/** @brief Oblicza zmianę liczby obszarów gracza wykonującego złoty ruch.
 * Zaczyna z wartością 1 (ponieważ złoty ruch powoduje potencjalne powstanie
 * nowego obszaru) i odejmuje liczbę różnych obszarów do których należą
 * przed złotym ruchem pola (@p x_coords[i], @p y_coords[i]) dla 0 <= i < 4,
 * jeśli stoi na nich pionek gracza @p new_owner.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g     - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] new_owner - numer gracza wykonującego złoty ruch, liczba nieujemna,
 * @param[in] x_coords  - tablica numerów kolumn pól przyległych do pola
 *                        na które wykonywany jest złoty ruch, liczba nieujemna,
 * @param[in] y_coords  - tablica numerów wierszy pól przyległych do pola
 *                        na które wykonywany jest złoty ruch, liczba nieujemna,
 * @return Liczba, o jaką zmieniła się liczba obszarów gracza @p new_owner
 * w wyniku wykonania przez niego złotego ruchu.
 */
static int calculate_new_owner_occ_areas_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]) {
    int new_owner_occupied_areas_change = 1;
    field_t *adjacent_areas[DIRECTIONS];
    unsigned quantity = 0;
    bool already_considered;
    field_t *representative;
    uint32_t x_;
    uint32_t y_;

    for (unsigned i = 0; i < DIRECTIONS; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (belongs_to_player(g, new_owner, x_, y_)) {
            representative = find_representative(get_field_ptr(g, x_, y_));
            already_considered = false;
            for (unsigned j = 0; j < quantity; ++j) {
                if (adjacent_areas[j] == representative) {
                    already_considered = true;
                    break;
                }
            }
            if (!already_considered)
                --new_owner_occupied_areas_change;
            adjacent_areas[quantity++] = representative;
        }
    }

    return new_owner_occupied_areas_change;
}

/** @brief Oblicza zmianę liczby obszarów gracza zaatakowanego w złotym ruchu.
 * Zaczyna z wartością -1 (ponieważ złoty ruch powoduje potencjalną utratę
 * jednego obszaru) i dodaje liczbę różnych obszarów do których należą
 * przed złotym ruchem pola (@p x_coords[i], @p y_coords[i]) dla 0 <= i < 4,
 * jeśli stoi na nich pionek gracza @p former_owner.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g         - wskaźnik na strukturę przechowującą stan gry,
 * @param[in] former_owner  - numer gracza atakowanego w złotym ruchu,
 *                            liczba nieujemna,
 * @param[in] x_coords      - tablica numerów kolumn pól przyległych do pola
 *                             na które wykonywany jest złoty ruch,
 *                             liczba nieujemna,
 * @param[in] y_coords      - tablica numerów wierszy pól przyległych do pola
 *                             na które wykonywany jest złoty ruch,
 *                             liczba nieujemna.
 * @return Liczba, o jaką zmieniła się liczba obszarów gracza @p player
 * w wyniku wykonania złotego ruchu na pole położone pomiędzy polami
 * (@p x_coords[i], @p y_coords[i]) dla 0 <= i < 4.
 */
static int calculate_former_owner_occ_areas_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]) {
    int former_owner_occupied_areas_change = -1;

    for (unsigned i = 0; i < DIRECTIONS; ++i) {
        if (start_dfs(g, TEST, former_owner,
                      x_coords[i], y_coords[i]))
            ++former_owner_occupied_areas_change;
    }

    return former_owner_occupied_areas_change;
}

gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas) {
    if (width == 0 || height == 0 || players == 0 || areas == 0)
        return NULL;

    gamma_t *g = malloc(sizeof(gamma_t));
    if (g == NULL)
        return NULL;
    player_t *players_data = calloc(players, sizeof(player_t));
    if (players_data == NULL) {
        free(g);
        return NULL;
    }
    board_t rows = malloc(sizeof(row_t) * height);
    if (rows == NULL) {
        free(g);
        free(players_data);
        return NULL;
    }
    for (uint32_t i = 0; i < height; ++i) {
        rows[i] = calloc(width, sizeof(field_t));
        if (rows[i] == NULL) {
            free(g);
            free(players_data);
            for (uint32_t j = 0; j < i; ++j) {
                free(rows[i]);
            }
            free(rows);
            return NULL;
        }
    }
    g->height = height;
    g->width = width;
    g->max_areas = areas;
    g->players_num = players;
    g->free_fields = (uint64_t)height * (uint64_t)width;
    g->players = players_data;
    g->board = rows;
    g->players_num_len = snprintf(NULL, 0, "%-" PRIu32, players);
    if (g->players_num_len >= 2)
        ++(g->players_num_len);

    return g;
}

void gamma_delete(gamma_t *g) {
    if (g == NULL)
        return;
    free(g->players);
    for (uint32_t i = 0; i < g->height; ++i) {
        free(g->board[i]);
    }
    free(g->board);
    free(g);
}

unsigned gamma_get_max_player_len(gamma_t *g) {
    return g->players_num_len;
}

uint32_t gamma_get_players_num(gamma_t *g) {
    return g->players_num;
}

uint32_t gamma_get_width(gamma_t *g) {
    return g->width;
}

uint32_t gamma_get_height(gamma_t *g) {
    return g->height;
}

uint32_t gamma_get_owner(gamma_t *g, uint32_t x, uint32_t y) {
    if (is_occupied(g, x, y))
        return g->board[y][x].occupied_by + 1;
    else
        return 0;
}

uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    --player;
    if (g == NULL || !valid_player(g, player))
        return 0;
    else
        return g->players[player].taken_fields;
}

uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    --player;
    if (g == NULL || !valid_player(g, player)) {
        return 0;
    }
    else if (g->players[player].occupied_areas < g->max_areas) {
        return g->free_fields;
    }
    else {
        return g->players[player].available_adj_fields;
    }
}

bool gamma_move_possible_there(gamma_t *g, uint32_t player, uint32_t x,
                               uint32_t y) {
    --player;
    if (g == NULL || !valid_coords(g, x, y) || !valid_player(g, player))
        return false;
    else if (is_occupied(g, x, y)
            || (g->players[player].occupied_areas == g->max_areas
                && !has_neighbour_of_player(g, player, x, y)))
        return false;
    else
        return true;
}

bool gamma_golden_possible_there(gamma_t *g, uint32_t attacker,
                                 uint32_t x, uint32_t y) {
    --attacker;
    if (g == NULL || !valid_coords(g, x, y) || !valid_player(g, attacker))
        return false;
    if (!is_occupied(g, x, y) || belongs_to_player(g, attacker, x, y)
        || g->players[attacker].golden_move_used
        || (g->players[attacker].occupied_areas == g->max_areas
            && !has_neighbour_of_player(g, attacker, x, y)))
        return false;

    uint32_t owner = get_owner(g, x, y);
    uint32_t x_coords[DIRECTIONS];
    uint32_t y_coords[DIRECTIONS];

    x_coords[UP]    = x     ; y_coords[UP]    = y + 1;
    x_coords[RIGHT] = x + 1 ; y_coords[RIGHT] = y    ;
    x_coords[DOWN]  = x     ; y_coords[DOWN]  = y - 1;
    x_coords[LEFT]  = x - 1 ; y_coords[LEFT]  = y    ;

    set_visited(g, true, x, y);
    int change = calculate_former_owner_occ_areas_change(
                   g, owner, x_coords, y_coords);

    set_visited(g, false, x, y);
    for (unsigned i = 0; i < DIRECTIONS; ++i) {
        start_dfs(g, UNDO, owner,
                  x_coords[i], y_coords[i]);
    }
    return g->players[owner].occupied_areas + change <= g->max_areas;
}

bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    --player;
    if (g == NULL || !valid_player(g, player)
        || g->players[player].golden_move_used)
        return false;

    bool any_target_exists = false;
    for (uint32_t i = 0; i < g->players_num; ++i) {
        if (g->players[i].taken_fields > 0 && i != player) {
            if (g->players[player].occupied_areas < g->max_areas) {
                return true;
            }
            else {
                any_target_exists = true;
                break;
            }
        }
    }
    if (any_target_exists) {
        for (uint32_t y = 0; y < g->height; ++y) {
            for (uint32_t x = 0; x < g->width; ++x) {
                if (gamma_golden_possible_there(g, player + 1, x, y))
                    return true;
            }
        }
    }
    return false;
}

bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (!gamma_move_possible_there(g, player, x, y))
        return false;
    --player;

    // Wykonanie ruchu.
    ++g->players[player].taken_fields;
    ++g->players[player].occupied_areas;
    --g->free_fields;

    // Zainicjowanie nowego obszaru.
    field_t *representative = get_field_ptr(g, x, y);
    set_parent(representative, representative);

    uint32_t x_coords[DIRECTIONS] = {x, x + 1, x, x - 1};
    uint32_t y_coords[DIRECTIONS] = {y + 1, y, y - 1, y};
    uint32_t x_;
    uint32_t y_;

    uint32_t players_to_decrement[DIRECTIONS];
    unsigned players_to_decrement_quantity = 0;
    bool player_already_decremented;
    field_t *neigh_areas[DIRECTIONS];
    unsigned neigh_areas_quantity = 0;
    bool area_already_considered;
    uint32_t owner;

    for (unsigned i = 0; i < DIRECTIONS; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (!valid_coords(g, x_, y_))
            continue;

// Zmniejszenie licznika pustych przyległych pól każdemu sąsiedniemu graczowi.
        if (is_occupied(g, x_, y_)) {
            owner = get_owner(g, x_, y_);
            player_already_decremented = false;
            for (unsigned j = 0; j < players_to_decrement_quantity; ++j) {
                if (players_to_decrement[j] == owner) {
                    player_already_decremented = true;
                    break;
                }
            }
            if (!player_already_decremented) {
                --g->players[owner].available_adj_fields;
                players_to_decrement[players_to_decrement_quantity++] = owner;
            }
        }
// Obliczenie zmiany liczby pustych przyległych pól gracza wykonującego ruch.
        else if (!has_neighbour_of_player(g, player, x_, y_)) {
            ++g->players[player].available_adj_fields;
        }
        // Aktualizacja struktury Find & Union.
        if (belongs_to_player(g, player, x_, y_)) {
            representative = find_representative(get_field_ptr(g, x_, y_));
            area_already_considered = false;
            for (unsigned j = 0; j < neigh_areas_quantity; ++j) {
                if (neigh_areas[j] == representative) {
                    area_already_considered = true;
                    break;
                }
            }
            if (!area_already_considered) {
                --g->players[player].occupied_areas;
                neigh_areas[neigh_areas_quantity++] = representative;
            }
            unite_areas(g, x_, y_, x, y);
        }
    }
    set_owner(g, player, x, y);

    return true;
}

bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    --player;
    if (g == NULL || !valid_coords(g, x, y) || !valid_player(g, player))
        return false;
    if (g->players[player].golden_move_used)
        return false;
    if (!is_occupied(g, x, y) || belongs_to_player(g, player, x, y))
        return false;
    if (g->players[player].occupied_areas == g->max_areas
        && !has_neighbour_of_player(g, player, x, y))
        return false;

    uint32_t former_owner = get_owner(g, x, y);
    uint32_t new_owner = player;
    uint32_t x_coords[DIRECTIONS] = {x, x + 1, x, x - 1};
    uint32_t y_coords[DIRECTIONS] = {y + 1, y, y - 1, y};

    int32_t new_owner_adj_fields_change =
            calculate_player_adj_free_fields_change(
                    g, true, new_owner, x_coords, y_coords);
    int32_t new_owner_occupied_areas_change =
            calculate_new_owner_occ_areas_change(
                    g, new_owner, x_coords, y_coords);

// Pominięcie DFSów z czasem liniowym, gdy wiadomo że złoty ruch jest niemożliwy.
    if (g->players[new_owner].occupied_areas + new_owner_occupied_areas_change
            > g->max_areas)
        return false;

    // Próbny złoty ruch.
    bool succeeded;
    set_owner(g, new_owner, x, y);
    set_visited(g, true, x, y);

    int32_t former_owner_adj_fields_change =
            calculate_player_adj_free_fields_change(
                    g, false, former_owner, x_coords, y_coords);
    int32_t former_owner_occupied_areas_change =
            calculate_former_owner_occ_areas_change(
                    g, former_owner, x_coords, y_coords);

    // Sprawdzenie ostatniego warunku koniecznego dla złotego ruchu.
    if (g->players[former_owner].occupied_areas +
        former_owner_occupied_areas_change > g->max_areas) {
        set_owner(g, former_owner, x, y);
        succeeded = false;
    }
    else {
        // Zapisanie zmian stanu gry w wyniku pomyślnego złotego ruchu.
        g->players[new_owner].occupied_areas +=
                new_owner_occupied_areas_change;
        g->players[former_owner].occupied_areas +=
                former_owner_occupied_areas_change;
        g->players[new_owner].available_adj_fields +=
                new_owner_adj_fields_change;
        g->players[former_owner].available_adj_fields +=
                former_owner_adj_fields_change;
        ++g->players[new_owner].taken_fields;
        --g->players[former_owner].taken_fields;
        g->players[new_owner].golden_move_used = true;

        for (unsigned i = 0; i < DIRECTIONS; ++i) {
            if (belongs_to_player(g, new_owner, x_coords[i], y_coords[i])) {
                unite_areas(g, x, y, x_coords[i], y_coords[i]);
            }
        }
        succeeded = true;
    }
    // Powrót do stanu wyjściowego flag odwiedzin sprzed uruchomienia
    // algorytmu przeszukującego oraz, jeśli złoty ruch się powiódł,
    // zapis zmian w strukturze obszarów.
    set_visited(g, false, x, y);
    for (unsigned i = 0; i < DIRECTIONS; ++i) {
        start_dfs(g, succeeded ? UNITE : UNDO, former_owner,
                  x_coords[i], y_coords[i]);
        }

    return succeeded;
}

char* gamma_board(gamma_t *g) {
    if (g == NULL)
        return NULL;
    char *board;
    char *curr_pos;
    if (true) {
        board = malloc((g->height) * (g->width + 1) * sizeof(char)
                       * (g->players_num_len) + 1);
        curr_pos = board;
        for (uint32_t y = g->height - 1; y < g->height; --y) {
            for (uint32_t x = 0; x < g->width; ++x) {
                assert(valid_coords(g, x, y));
                if (is_occupied(g, x, y)) {
                    sprintf(curr_pos, "%*" PRIu32, g->players_num_len,
                            get_owner(g, x, y) + 1);
                }
                else {
                    sprintf(curr_pos, "%*c", g->players_num_len, '.');
                }
                curr_pos += g->players_num_len;
            }
            *(curr_pos++) = '\n';
        }
    }
    *curr_pos = '\0';
    return board;
}
