/** @file
 * Interfejs klasy zawierającej struktury, typy
 * i procedury pomocnicze gry gamma.
 * Procedury tu zdefiniowane przyjmują nieujemny numer gracza,
 * mniejszy o 1 od faktycznego.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2.04.2020
 */


#ifndef GAMMA_UTILS_H
#define GAMMA_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>


/**
 * Typ przedstawiający położenie pól względem siebie.
 */
typedef enum {UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3, RET = 4} direction_t;

/**
 * Struktura przechowująca stan pola na planszy.
 * Implementuje także strukturę Find & Union na planszy. Działa ona następująco:
 * Każde pole należące do spójnego obszaru gracza ma przydzielonego ojca,
 * czyli pewne pole należące również do tego obszaru. Któryś z kolei ojciec sam jest
 * swoim ojcem - nazywa się go przodkiem. Do jego znalezienia służy @ref find_representative.
 * Aby zapobiec tworzeniu się zbyt długich łańcuchów prowadzących do przodka, każde wywołanie
 * @ref find_representative dokonuje kompresji ścieżek, czyli ustawia przodka jako bezpośredniego
 * ojca wszystkich pól, po których nastąpiło do niego dojście. Łączenie obszarów zapewnia
 * @ref unite_areas, zaś dzielenie jest możliwe tylko przez utworzenie ich od nowa poprzez
 * użycie algorytmu przeszukującego @ref start_dfs.
 */
struct field {
    bool occupied;          ///< czy na polu stoi pionek
    uint32_t occupied_by;   ///< czyj pionek stoi na polu
    bool visited;           ///< czy już odwiedzone przez algorytm przeszukujący
    direction_t base_dir;   ///< skąd pierwotnie pojawił się na tym polu algorytm przeszukujący
    struct field *parent;   ///< wskaźnik na pole-ojca w obrębie tego samego obszaru
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
 * Struktura przechowująca dane na temat gracza.
 */
struct player {
    bool golden_move_available;         ///< czy gracz nie wykonał jeszcze złotego ruchu
    uint32_t occupied_areas;            ///< ile obszarów zajmuje gracz na planszy
    uint64_t taken_fields;              ///< ile pól zajmuje gracz na planszy
    uint64_t available_adjacent_fields; ///< ile pustych pól przylega do pionków gracza
};

/**
 * Typ przedstawiający dane na temat gracza.
 */
typedef struct player player_t;

/**
 * Struktura przechowująca stan gry.
 */
struct gamma {
    uint32_t height;        ///< wysokość planszy
    uint32_t width;         ///< szerokość planszy
    uint32_t players_num;   ///< liczba graczy biorących udział w rozgrywce
    uint32_t max_areas;     ///< maksymalna dopuszczalna liczba obszarów
                            ///< dla każdego gracza
    uint64_t free_fields;   ///< liczba pustych pól na planszy
    player_t *players;      ///< tablica indywidualnych konfiguracji graczy, przechowuje gracza
                            ///< z numerem @p n w miejscu o indeksie @p n-1
    board_t board;          ///< plansza do gry
};

/**
 * Typ przedstawiający stan gry.
 */
typedef struct gamma gamma_t;

/**
 * Typ trybu działania algorytmu przeszukującego obszary zaatakowanego w złotym ruchu gracza
 *  sąsiadujące z zaatakowanym polem.
 */
typedef enum {TEST = 0, UNDO = 1, UNITE = 2} dfs_mode;


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

/** @brief Ustawia podanego gracza jako zajmującego pole.
 * Ustawia gracza @p player jako zajmującego pole (@p x, @p y).
 * Zakłada, że @p g jest poprawną konfiguracją gry, że gracz @p player
 * bierze udział w rozgrywce oraz że pole (@p x, @p y) y
 * znajduje się na planszy zadanej w @p g.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] player  - numer gracza, liczba nieujemna mniejsza od wartości
 *                      @p players z funkcji @ref gamma_new,
 * @param[in] x       – numer kolumny, liczba nieujemna mniejsza od wartości
 *                      @p width z funkcji @ref gamma_new,
 * @param[in] y       – numer wiersza, liczba nieujemna mniejsza od wartości
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
 * przez @p child_ptr. Zakłada, że parent_ptr oraz child_ptr wskazują
 * na poprawne pola na planszy zadanej w @ref gamma_new.
 * @param[in] parent_ptr    – wskaźnik na pole-ojca
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
 * @param[in,out] field_ptr – wskaźnik na pole na planszy zadanej
 *                              w @ref gamma_new.
 * @return Wskaźnik na reprezentanta obszaru do którego należy
 * pole wskazywane przez @p field_ptr.
 */
field_t* find_representative(field_t *field_ptr);

/** @brief Łączy obszary, do których należą dane dwa pola.
 * Ustawia reprezentanta obszaru, do którego należy pole (@p x1, @p y1),
 * jako reprezentanta obszaru do którego należy pole (@p x2, @p y2).
 * Zakłada, że @p g jest poprawną konfiguracją gry oraz że (@p x1, @p y1)
 * i (@p x2, @p y2) są poprawnymi polami na planszy zadanej w @p g.
 * @param[in,out] g  – wskaźnik na strukturę przechowującą stan gry,
 * @param[in] x1     – numer kolumny pola ojcowskiego, liczba nieujemna
 *                     mniejsza od wartości @p width z funkcji @ref gamma_new,
 * @param[in] y1     - numer wiersza pola ojcowskiego, liczba nieujemna
 *                     mniejsza od wartości @p height z funkcji @ref gamma_new,
 * @param[in] x2     – numer kolumny pola synowskiego, liczba nieujemna
 *                     mniejsza od wartości @p width z funkcji @ref gamma_new,
 * @param[in] y2     – numer wiersza pola synowskiego, liczba nieujemna
 *                     mniejsza od wartości @p height z funkcji @ref gamma_new.
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
static inline bool was_visited(gamma_t *g, dfs_mode mode, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return (mode == TEST) ? g->board[y][x].visited : !g->board[y][x].visited;
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
    g->board[y][x].visited = flag;
}

/** @brief Uruchamia na danym polu algorytm przeszukiwania obszaru danego gracza.
 * Przechodzi po całym obszarze w którego skład wchodzi pole (@p x, @p y)
 * w trybie @p mode, jeśli znajduje się na nim pionek gracza @p player.
 * W trybie @p TEST przechodzi po polach z flagą @p visited ustawioną na false,
 * zaś w trybach @p UNDO i @p UNITE po polach flagą @p visited ustawioną na true.
 * W trybie UNITE dodatkowo łączy odwiedzane pola w jeden spójny obszar.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g   – wskaźnik na strukturę przechowującą stan gry
 * @param[in] mode    - tryb pracy algorytmu przeszukującego, jedna
 *                       wartości typu @ref bfs_mode,
 * @param[in] player  - numer gracza, liczba nieujemna,
 * @param[in] x       – numer kolumny, liczba nieujemna,
 * @param[in] y       – numer wiersza, liczba nieujemna.
 * @return W trybie @p TEST: wartość @p true, jeśli pole (@p x, @p y) należy do
 * gracza @p player oraz nie zostało jeszcze odwiedzone,
 * a @p false w przeciwnym przypadku. W trybach @p UNDO oraz @p UNITE:
 * wartość @p true, jeśli pole (@p x, @p y) należy do gracza @p player
 * oraz zostało już odwiedzone, a @p false w pozostałych przypadkach.
 */
bool start_dfs(gamma_t *g, dfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y);

/** @brief Oblicza zmianę liczby obszarów gracza wykonującego złoty ruch.
 * Zaczyna z wartością 1 (ponieważ złoty ruch powoduje potencjalne powstanie
 * nowego obszaru) i odejmuje liczbę różnych obszarów do których należą
 * przed złotym ruchem pola (@p x_coords[i], @p y_coords[i]) dla 0 <= i < 4,
 * jeśli stoi na nich pionek gracza @p new_owner.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g     - wskaźnik na strukturę przechowującą stan gry
 * @param[in] new_owner - numer gracza wykonującego złoty ruch, liczba nieujemna
 * @param[in] x_coords  - tablica numerów kolumn pól przyległych do pola
 *                        na które wykonywany jest złoty ruch, liczba nieujemna
 * @param[in] y_coords  - tablica numerów wierszy pól przyległych do pola
 *                        na które wykonywany jest złoty ruch, liczba nieujemna
 * @return Liczba, o jaką zmieniła się liczba obszarów gracza @p new_owner
 * w wyniku wykonania przez niego złotego ruchu.
 */
int32_t calculate_new_owner_occ_areas_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]);

/** @brief Oblicza zmianę liczby pustych pól przyległych do obszarów gracza
 * atakującego lub atakowanego w złotym ruchu.
 * Zlicza pola (@p x_coords[i], @p y_coords[i]) dla 0 <= i < 4, jeśli przed
 * złotym ruchem nie sąsiadują z żadnym polem zajętym przez gracza @p player.
 * Jeśli wartością @p count_for_new_owner jest @p true, wówczas zwracany jest
 * nieujemny wynik, a w przeciwnym przypadku zwracana jest liczba przeciwna
 * do tego nieujemnego wyniku. Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g               – wskaźnik na strukturę przechowującą stan gry
 * @param[in] count_for_new_owner - czy obliczenia dotyczą gracza atakującego,
 * @param[in] player              - numer gracza atakowanego lub atakującego
 *                                  w złotym ruchu, liczba nieujemna,
 * @param[in] x_coords            – tablica numerów kolumn pól przyległych do pola
 *                                  na które wykonywany jest złoty ruch, liczba nieujemna,
 * @param[in] y_coords            – tablica numerów wierszy pól przyległych do pola
 *                                  na które wykonywany jest złoty ruch, liczba nieujemna.
 * @return Liczba, o jaką zmieniła się liczba pustych pól przyległych
 * do obszarów gracza @p player.
 */
int32_t calculate_player_adj_free_fields_change(gamma_t *g, bool count_for_new_owner,
        uint32_t player, uint32_t *x_coords, uint32_t *y_coords);

/** @brief Oblicza zmianę liczby obszarów gracza zaatakowanego w złotym ruchu.
 * Zaczyna z wartością -1 (ponieważ złoty ruch powoduje potencjalną utratę
 * jednego obszaru) i dodaje liczbę różnych obszarów do których należą
 * przed złotym ruchem pola (@p x_coords[i], @p y_coords[i]) dla 0 <= i < 4,
 * jeśli stoi na nich pionek gracza @p former_owner.
 * Zakłada, że @p g jest poprawną konfiguracją gry.
 * @param[in,out] g         – wskaźnik na strukturę przechowującą stan gry
 * @param[in] former_owner  - numer gracza atakowanego w złotym ruchu,
 *                            liczba nieujemna,
 * @param[in] x_coords      – tablica numerów kolumn pól przyległych do pola
 *                             na które wykonywany jest złoty ruch, liczba nieujemna,
 * @param[in] y_coords      – tablica numerów wierszy pól przyległych do pola
 *                             na które wykonywany jest złoty ruch, liczba nieujemna.
 * @return Liczba, o jaką zmieniła się liczba obszarów gracza @p player
 * w wyniku wykonania złotego ruchu na pole położone pomiędzy polami
 * (@p x_coords[i], @p y_coords[i]) dla 0 <= i < 4.
 */
int32_t calculate_former_owner_occ_areas_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]);


#endif //GAMMA_UTILS_H
