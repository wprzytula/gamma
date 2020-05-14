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
#include <inttypes.h>
#include <termios.h>
#include <unistd.h>

#define ANSI "\x1b["        ///< sekwencja rozpoczynająca kod ANSI
#define ASCII_EOF 4         ///< numer znaku EOF w kodzie ASCII
#define ASCII_ESC 27        ///< numer znaku ESC w kodzie ASCII

/**
 * Kierunek ruchu po planszy w związku z naciśniętym klawiszem strzałki.
 */
typedef enum {LEFT = 'D', UP = 'A', DOWN = 'B', RIGHT = 'C'} arrow;

/** @brief Aktualizuje wygląd planszy.
 * Umieszcza numer gracza @p player w odpowiednim miejscu ekranu,
 * reprezentującym zadane współrzędnymi (@p x, @p y)
 * pole na planszy rozgrywki @p g.
 * @param[in] g       - konfiguracja gry gamma,
 * @param[in] x       - współrzędna pozioma pola na planszy,
 * @param[in] y       - współrzędna pionowa pola na planszy,
 * @param[in] player  - numer gracza, którego pionek zajmuje dane pole.
 */
static void update_board(gamma_t *g, uint32_t x, uint32_t y, uint32_t player) {
    printf(ANSI"s"); // Zapisuje położenie kursora.

    // Przesuwa kursor w dane miejsce na ekranie
    printf(ANSI"%u;%uH", gamma_get_height(g) - y,
            1 + x * gamma_get_max_player_len(g));
    printf("%*" PRIu32, gamma_get_max_player_len(g), player);

    printf(ANSI"u"); // Przywraca zapisane położenie kursora.
}

/** @brief Wyświetla podsumowanie zakończonej rozgrywki.
 * Dla każdego gracza biorącego udział w rozgrywce @p g drukuje pod planszą
 * wiersz zawierający jego numer oraz liczbę zajętych przez niego pól.
 * @param[in] g     - konfiguracja gry gamma.
 */
static void show_summary(gamma_t *g) {
    // Przesuwa kursor w dane miejsce na ekranie
    printf(ANSI"%u;%uH", gamma_get_height(g) + 1, 0);
    printf(ANSI"K"); // Czyści zawartość bieżącej linii.
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

/** @brief Wyświetla statystyki dla danego gracza.
 * Wyświetla pod planszą wiersz zawierający numer gracza obecnie wykonującego
 * ruch (@p curr_player), a także informację o liczbie zajętych przez
 * niego pól, informację o liczbie wolnych pól możliwych przez niego do zajęcia
 * w wykonywanym ruchu oraz informację o możliwości wykonania złotego ruchu.
 * @param[in] g             - konfiguracja gry gamma,
 * @param[in] curr_player   - numer gracza mającego teraz swoją kolej.
 */
static void update_stats(gamma_t *g, uint32_t curr_player) {
    printf(ANSI"s"); // Zapisuje położenie kursora.

    printf(ANSI"%u;%uH", gamma_get_height(g) + 1, 0); // Przesuwa kursor.
    printf(ANSI"K"); // Czyści zawartość bieżącej linii.
    printf("PLAYER %u B%lu F%lu%s", curr_player,
            gamma_busy_fields(g, curr_player),
            gamma_free_fields(g, curr_player),
            gamma_golden_possible(g, curr_player) ? " G" : "");

    printf(ANSI"u"); // Przywraca zapisane położenie kursora.
}

/** @brief Przesuwa kursor o jedno pole w zadanym kierunku.
 * Przesuwa kursor na pole sąsiadujące z polem (@p x, @p y) od strony
 * @p dirchr. Jeśli ruch spowodowałby wyjście poza planszę,
 * nie jest wykonywany.
 * @param[in] g          - konfiguracja gry gamma,
 * @param[in,out] x      - współrzędna pozioma pola, na którym
 *                         znajduje się kursor,
 * @param[in,out] y      - współrzędna pionowa pola, na którym
 *                         znajduje się kursor,
 * @param[in] dirchr     - kierunek wykonania ruchu kursorem.
 */
static void move_cursor(gamma_t *g, uint32_t *x, uint32_t *y, arrow dirchr) {
    switch (dirchr) {
        case UP:
            if (*y < gamma_get_height(g) - 1) {
                printf(ANSI"1A"); // Przesuwa kursor w górę.
                ++*y;
            }
            break;

        case DOWN:
            if (*y > 0) {
                printf(ANSI"1B"); // Przesuwa kursor w dół.
                --*y;
            }
            break;
        case RIGHT:
            if (*x < gamma_get_width(g) - 1) {
                // Przesuwa kursor w prawo.
                printf(ANSI"%uC", gamma_get_max_player_len(g));
                ++*x;
            }
            break;
        case LEFT:
            if (*x > 0) {
                // Przesuwa kursor w lewo.
                printf(ANSI"%uD", gamma_get_max_player_len(g));
                --*x;
            }
            break;
        default:
            break;
    }
}

/** @brief Przeprowadza turę gracza.
 * Pobiera znaki podawane przez gracza na wejście i wykonuje związane z nimi
 * akcje tak długo, aż gracz @p player wykona prawidłową turę lub zakończy
 * rozgrywkę. Jeśli zostanie naciśnięty klawisz dowolnej strzałki, to kursor
 * jest przesuwany o jedno pole w stosowną stronę (pod warunkiem że napotka tam
 * kolejne pole na planszy). Po umieszczeniu kursora na danym polu gracz może
 * spróbować wykonać zwykły ruch naciskając spację lub złoty ruch naciskając
 * 'G' lub 'g'. Jeśli ruch się powiedzie, jego tura się zakończy a funkcja
 * skończy działanie; w przeciwnym wypadku nic się nie stanie i tura będzie
 * kontynuowana. Naciśnięcie klawisza 'c' lub 'C' spowoduje pominięcie tury,
 * zaś kombinacji klawiszy Ctrl-D zakończenie rozgrywki - obydwa przypadki
 * kończą działanie funkcji.
 * @param[in,out] g    - konfiguracja gry gamma,
 * @param[in] player   - numer gracza mającego teraz swoją kolej na ruch,
 * @param[in,out] x    - współrzędna pozioma pola, na którym
 *                         znajduje się kursor,
 * @param[in,out] y    - współrzędna pionowa pola, na którym
 *                         znajduje się kursor.
 * @return Wartość @p true, jeśli rozgrywka ma być kontynuowana lub
 * @p false jeśli rozgrywka została zakończona kombinacją klawiszy Ctrl-D.
 */
static bool player_turn(gamma_t *g, uint32_t player, uint32_t *x, uint32_t *y) {
    int chr = -1;
    int lastchr = -1;
    int beforelastchr;
    while (true) {
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
            case 'B':
            case 'C':
            case 'D':
                if (beforelastchr == ASCII_ESC && lastchr == '[')
                    move_cursor(g, x, y, chr);
                else if (chr == 'C')
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

    printf(ANSI"2J"); // Czyści ekran.

    struct termios normal_term, inter_term;
    tcgetattr(STDIN_FILENO, &normal_term);
    inter_term = normal_term;

    // Przestawia konsolę w tryb niekanoniczny
    // i wyłącza wyświetlanie wprowadzanych znaków.
    inter_term.c_lflag = ~(ICANON | ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &inter_term);

    printf(ANSI"0;0H"); // Przesuwa kursor w lewy górny róg ekranu.
    char *board = gamma_board(g);
    if (!board) {
        gamma_delete(g);
        exit(1);
    }
    printf("%s", board);
    free(board);
    // Przesuwa kursor na pole w lewym górnym rogu planszy.
    printf(ANSI"0;%uH", gamma_get_max_player_len(g));

    uint32_t curr_player = 1;
    uint32_t last_able = 0;
    while (true) {
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

    // Przywraca poprzednie ustawienia konsoli.
    tcsetattr(STDIN_FILENO, TCSANOW, &normal_term);

    printf(ANSI"2J");
    printf(ANSI"0;0H");
}

