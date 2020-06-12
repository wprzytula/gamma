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
#include <sys/ioctl.h>

#define ASCII_EOF 4                 ///< Numer znaku EOF w kodzie ASCII.
#define ASCII_ESC 27                ///< Numer znaku ESC w kodzie ASCII.

#define ANSI "\x1b["                ///< Sekwencja rozpoczynająca kod ANSI.
#define SAVE "s"                    ///< Zapisuje położenie kursora.
#define RESTORE "u"                 ///< Przywraca zapisane położenie kursora.
#define CLEAR_LINE "K"              ///< Czyści zawartość bieżącej linii.
#define CLEAR_SCREEN "2J"           ///< Czyści zawartość ekranu.
#define YELLOW "33m"                ///< Kod ANSI koloru żółtego.
#define GREEN "32m"                 ///< Kod ANSI koloru zielonego.
#define CYAN "36m"                  ///< Kod ANSI koloru błękitnego.
#define CURRENT_PLAYER_COLOR CYAN   ///< Kolor pionków aktualnego gracza.
#define POSSIBLE_FIELD_COLOR GREEN  ///< Kolor pól dostępnych dla ruchu gracza.
#define GOLDEN_COLOR YELLOW         ///< Kolor pionków dostępnych dla
                                    ///< wykonania złotego ruchu.
#define REVERT "0m"                 ///< Kod ANSI powrotu do koloru domyślnego.

/**
 * Kierunek ruchu po planszy w związku z naciśniętym klawiszem strzałki.
 */
typedef enum {LEFT = 'D', UP = 'A', DOWN = 'B', RIGHT = 'C'} arrow;

/**
 * @brief Przesuwa kursor w dane miejsce na ekranie.
 * @param[in] row      - numer rzędu, w którym na zostać umieszczony kursor,
 * @param[in] column   - numer kolumny, w której na zostać umieszczony kursor.
 */
static void place_cursor(uint32_t row, uint32_t column) {
    printf(ANSI"%u;%uH", row, column);
}

/** @brief Koloruje planszę dla gracza mającego obecnie ruch.
 * Jeśli flaga @p nonstandard jest true, wówczas koloruje na zielono puste pola,
 * na które obecnie mający kolejkę gracz może wykonać ruch; na błękitno pola
 * zajmowane przez tego gracza; na żółto pola, na które może wykonać złoty ruch;
 * pozostałe pola pozostają białe. Jeśli flaga @p nonstandard jest false,
 * koloruje wszystkie wymienione pola na biało.
 * @param[in] g            - konfiguracja gry gamma,
 * @param[in] player       - numer gracza, który obecnie ma ruch,
 * @param[in] nonstandard  - tryb kolorowania.
 */
static void colorize_board_for_current_player(gamma_t *g, uint32_t player,
                                              bool nonstandard) {
    printf(ANSI SAVE);

    for (uint32_t y = 0; y < gamma_get_height(g); ++y) {
        for (uint32_t x = 0; x < gamma_get_width(g); ++x) {
            place_cursor(gamma_get_height(g) - y,
                         1 + x * gamma_get_max_player_len(g));
            if (gamma_get_owner(g, x, y) == player) {
                if (nonstandard)
                    printf(ANSI CURRENT_PLAYER_COLOR);
                printf("%*" PRIu32, gamma_get_max_player_len(g), player);
            }
            else if (gamma_get_owner(g, x, y) == 0) {
                if (nonstandard && gamma_move_possible_there(g, player, x, y))
                    printf(ANSI POSSIBLE_FIELD_COLOR);
                printf("%*c", gamma_get_max_player_len(g), '.');
            }
            else {
                if (nonstandard && gamma_golden_possible_there(g, player, x, y))
                    printf(ANSI GOLDEN_COLOR);
                printf("%*u", gamma_get_max_player_len(g),
                        gamma_get_owner(g, x, y));
            }
            printf(ANSI REVERT);
        }
    }
    printf(ANSI RESTORE);
}

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
    printf(ANSI SAVE);

    place_cursor(gamma_get_height(g) - y,
            1 + x * gamma_get_max_player_len(g));
    printf("%*" PRIu32, gamma_get_max_player_len(g), player);

    printf(ANSI RESTORE);
}

/** @brief Wyświetla statystyki dla danego gracza.
 * Wyświetla pod planszą wiersz zawierający numer gracza obecnie wykonującego
 * ruch (@p curr_player), a także informację o liczbie zajętych przez
 * niego pól, informację o liczbie wolnych pól możliwych przez niego do zajęcia
 * w wykonywanym ruchu oraz informację o możliwości wykonania złotego ruchu.
 * @param[in] g        - konfiguracja gry gamma,
 * @param[in] player   - numer gracza mającego teraz swoją kolej.
 */
static void update_stats(gamma_t *g, uint32_t player) {
    printf(ANSI SAVE);

    place_cursor(gamma_get_height(g) + 1, 0);
    printf(ANSI CLEAR_LINE);
    printf("PLAYER %u ", player);
    printf(ANSI CURRENT_PLAYER_COLOR);
    printf("B%lu ", gamma_busy_fields(g, player));
    printf(ANSI POSSIBLE_FIELD_COLOR);
    printf("F%lu", gamma_free_fields(g, player));
    printf(ANSI GOLDEN_COLOR);
    printf(gamma_golden_possible(g, player) ? " G" : "");
    printf(ANSI REVERT);

    printf(ANSI RESTORE);
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
static void move_cursor_in_direction(gamma_t *g, uint32_t *x, uint32_t *y, arrow dirchr) {
    switch (dirchr) {
        case UP:
            if (*y < gamma_get_height(g) - 1) {
                // Przesuwa kursor jedno pole w górę.
                printf(ANSI"1A");
                ++*y;
            }
            break;

        case DOWN:
            if (*y > 0) {
                // Przesuwa kursor jedno pole w dół.
                printf(ANSI"1B");
                --*y;
            }
            break;
        case RIGHT:
            if (*x < gamma_get_width(g) - 1) {
                // Przesuwa kursor jedno pole w prawo.
                printf(ANSI"%uC", gamma_get_max_player_len(g));
                ++*x;
            }
            break;
        case LEFT:
            if (*x > 0) {
                // Przesuwa kursor jedno pole w lewo.
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
 * @p false, jeśli rozgrywka została zakończona kombinacją klawiszy Ctrl-D.
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
                    move_cursor_in_direction(g, x, y, chr);
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

/** @brief Wyświetla podsumowanie zakończonej rozgrywki.
 * Dla każdego gracza biorącego udział w rozgrywce @p g drukuje pod planszą
 * wiersz zawierający jego numer oraz liczbę zajętych przez niego pól.
 * Zwycięzców drukuje w kolorze, a także dodaje informację że wygrali.
 * @param[in] g   - konfiguracja gry gamma.
 */
static void show_summary(gamma_t *g) {
    place_cursor(gamma_get_height(g) + 1, 0);
    printf(ANSI CLEAR_LINE);
    uint64_t max_busy_fields = 0;
    uint32_t players = gamma_get_players_num(g);
    unsigned max_player_len = gamma_get_max_player_len(g);
    if (max_player_len > 2)
        --max_player_len;
    for (uint32_t player = 0; player++ < players;) {
        if (gamma_busy_fields(g, player) > max_busy_fields)
            max_busy_fields = gamma_busy_fields(g, player);
    }
    unsigned max_busy_fields_len = snprintf(NULL, 0, "%" PRIu64,
            max_busy_fields);
    for (uint32_t player = 0; player++ < players;) {
        if (gamma_busy_fields(g, player) == max_busy_fields) {
            printf(ANSI YELLOW);
            printf("PLAYER %*u HAS TAKEN: %*lu - WINS!\n", max_player_len,
                    player, max_busy_fields_len, gamma_busy_fields(g, player));
            printf(ANSI REVERT);
        }
        else {
            printf("PLAYER %*u HAS TAKEN: %*lu\n", max_player_len,
                    player, max_busy_fields_len, gamma_busy_fields(g, player));
        }
    }
}

void interactive_game(gamma_t *g) {
    uint32_t x = 0;
    uint32_t y = gamma_get_height(g) - 1;

    // Sprawdza, czy konsola ma wystarczające rozmiary by pomieścić planszę;
    // jeśli nie, przerywa program z kodem błędu.
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        gamma_delete(g);
        exit(1);
    }
    if (w.ws_col < gamma_get_width(g) * gamma_get_max_player_len(g)
        || w.ws_row < gamma_get_height(g) + 1) {
        printf("Za mały rozmiar terminala! Przerwanie działania.\n");
        gamma_delete(g);
        exit(1);
    }

    // Pobiera i wykonuje kopię poprzednich ustawień konsoli.
    struct termios normal_term, inter_term;
    if (tcgetattr(STDIN_FILENO, &normal_term) != 0) {
        gamma_delete(g);
        exit(1);
    }
    inter_term = normal_term;

    // Przestawia konsolę w tryb niekanoniczny
    // i wyłącza wyświetlanie wprowadzanych znaków.
    inter_term.c_lflag = ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &inter_term) != 0) {
        gamma_delete(g);
        exit(1);
    }

    printf(ANSI CLEAR_SCREEN);
    place_cursor(0,0);

    char *board = gamma_board(g);
    if (!board) {
        gamma_delete(g);
        exit(1);
    }
    printf("%s", board);
    free(board);

    // Przesuwa kursor na pole w lewym górnym rogu planszy.
    place_cursor(0, gamma_get_max_player_len(g));

    uint32_t curr_player = 1;
    uint32_t last_able = 0;
    while (true) {
        if (gamma_golden_possible(g, curr_player)
            || gamma_free_fields(g, curr_player) != 0) {
            last_able = curr_player;
            update_stats(g, curr_player);
            colorize_board_for_current_player(g, curr_player, true);
            if (!player_turn(g, curr_player, &x, &y)) {
                colorize_board_for_current_player(g, curr_player, false);
                break;
            }
            colorize_board_for_current_player(g, curr_player, false);
        }
        else if (last_able == curr_player)
            break;

        if (curr_player < gamma_get_players_num(g))
            ++curr_player;
        else
            curr_player = 1;
    }
    show_summary(g);

    // Przywraca poprzednie ustawienia konsoli.
    if (tcsetattr(STDIN_FILENO, TCSANOW, &normal_term) != 0) {
        gamma_delete(g);
        exit(1);
    }
}
