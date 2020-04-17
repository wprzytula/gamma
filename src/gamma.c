/** @file
 * Implementacja klasy przechowującej stan gry gamma.
 * Procedury tu zdefiniowane przyjmują dodatni numer gracza, natomiast
 * wewnętrznie przetwarzają go jako mniejszy o 1.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 25.03.2020
 */

#include "gamma.h"
#include <stdio.h>
#include <errno.h>


gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas) {
    if (width == 0 || height == 0 || players == 0 || areas == 0)
        return NULL;
    player_t *players_data = malloc(sizeof(player_t) * players);
    if (players_data == NULL)
        return NULL;
    for (uint32_t i = 0; i < players; ++i) {
        players_data[i] = (player_t){.golden_move_available = true,
                                    .occupied_areas = 0,
                                    .taken_fields = 0,
                                    .available_adj_fields = 0};
    }
    board_t rows = malloc(sizeof(row_t) * height);
    if (rows == NULL)
        return NULL;
    for (uint32_t i = 0; i < height; ++i) {
        rows[i] = malloc(sizeof(field_t) * width);
        if (rows[i] == NULL)
            return NULL;
        for (uint32_t j = 0; j < width; ++j) {
            rows[i][j] = (field_t){.visited = false,
                                   .occupied = false};
        }
    }
    gamma_t *g = malloc(sizeof(gamma_t));
    if (g == NULL)
        return NULL;
    g->height = height;
    g->width = width;
    g->max_areas = areas;
    g->players_num = players;
    g->free_fields = height * width;
    g->players = players_data;
    g->board = rows;

    return g;
}


void gamma_delete(gamma_t *g) {
    free(g->players);
    for (uint32_t i = 0; i < g->height; ++i) {
        free(g->board[i]);
    }
    free(g->board);
    free(g);
}


bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    --player;
    if (g == NULL || !valid_coords(g, x, y)
        || !valid_player(g, player))
        return false;
    if (is_occupied(g, x, y))
        return false;
    if (g->players[player].occupied_areas == g->max_areas
        && !has_neighbour_of_player(g, player, x, y))
        return false;

    // wykonanie ruchu
    ++g->players[player].taken_fields;
    ++g->players[player].occupied_areas;
    --g->free_fields;

    // zainicjowanie nowego obszaru
    field_t *representative = get_field_ptr(g, x, y);
    set_parent(representative, representative);

    uint32_t x_coords[4] = {x, x + 1, x, x - 1};
    uint32_t y_coords[4] = {y + 1, y, y - 1, y};
    uint32_t x_;
    uint32_t y_;

    uint32_t players_to_decrement[4];
    uint8_t players_to_decrement_quantity = 0;
    bool player_already_decremented;
    field_t *neigh_areas[4];
    uint8_t neigh_areas_quantity = 0;
    bool area_already_considered;
    uint32_t owner;

    for (uint8_t i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (!valid_coords(g, x_, y_))
            continue;

        // decrement every neighbouring player free adjacent fields counter
        if (is_occupied(g, x_, y_)) {
            owner = get_owner(g, x_, y_);
            player_already_decremented = false;
            for (uint8_t j = 0; j < players_to_decrement_quantity; ++j) {
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
        // calculate the change of current player free adjacent fields
        else if (!has_neighbour_of_player(g, player, x_, y_)) {
            ++g->players[player].available_adj_fields;
        }
        // find & union
        if (belongs_to_player(g, player, x_, y_)) {
            representative = find_representative(get_field_ptr(g, x_, y_));
            area_already_considered = false;
            for (uint8_t j = 0; j < neigh_areas_quantity; ++j) {
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
    if (!g->players[player].golden_move_available)
        return false;
    if (!is_occupied(g, x, y) || belongs_to_player(g, player, x, y))
        return false;
    if (g->players[player].occupied_areas == g->max_areas
        && !has_neighbour_of_player(g, player, x, y))
        return false;

    uint32_t former_owner = get_owner(g, x, y);
    uint32_t new_owner = player;
    uint32_t x_coords[4] = {x, x + 1, x, x - 1};
    uint32_t y_coords[4] = {y + 1, y, y - 1, y};

    int32_t new_owner_adj_fields_change =
            calculate_player_adj_free_fields_change(
                    g, true, new_owner, x_coords, y_coords);
    int32_t new_owner_occupied_areas_change =
            calculate_new_owner_occ_areas_change(
                    g, new_owner, x_coords, y_coords);

    // pominięcie DFSów z czasem liniowym, gdy już wiadomo że złoty ruch jest niemożliwy
    if (g->players[new_owner].occupied_areas + new_owner_occupied_areas_change
            > g->max_areas)
        return false;

    // próbny złoty ruch
    bool succeeded;
    set_owner(g, new_owner, x, y);
    set_visited(g, true, x, y);

    int32_t former_owner_adj_fields_change =
            calculate_player_adj_free_fields_change(
                    g, false, former_owner, x_coords, y_coords);
    int32_t former_owner_occupied_areas_change =
            calculate_former_owner_occ_areas_change(
                    g, former_owner, x_coords, y_coords);

    // sprawdzenie ostatniego warunku koniecznego dla złotego ruchu
    if (g->players[former_owner].occupied_areas +
        former_owner_occupied_areas_change > g->max_areas) {
        set_owner(g, former_owner, x, y);
        succeeded = false;
    }
    else {
        // zapisanie zmian stanu gry w wyniku pomyślnego złotego ruchu
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
        g->players[new_owner].golden_move_available = false;

        for (uint8_t i = 0; i < 4; ++i) {
            if (belongs_to_player(g, new_owner, x_coords[i], y_coords[i])) {
                unite_areas(g, x, y, x_coords[i], y_coords[i]);
            }
        }
        succeeded = true;
    }
    // powrót do stanu wyjściowego sprzed uruchomienia algorytmu przeszukującego
    // oraz, jeśli złoty ruch się powiódł, zapis zmian w strukturze obszarów
    set_visited(g, false, x, y);
    for (uint8_t i = 0; i < 4; ++i) {
        start_dfs(g, succeeded ? UNITE : UNDO, former_owner,
                  x_coords[i], y_coords[i]);
        }

    return succeeded;
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


bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    --player;
    if (g != NULL && valid_player(g, player)
        && g->players[player].golden_move_available) {
        for (uint32_t i = 0; i < g->players_num; ++i) {
            if (g->players[i].taken_fields > 0 && i != player)
                return true;
        }
    }
    return false;
}


char* gamma_board(gamma_t *g) {
    if (g == NULL)
        return NULL;
    char *board;
    uint64_t index = 0;
    if (g->players_num <= 9) {
        board = malloc((g->height) * (g->width + 1) * sizeof(char) + 1);
        for (uint32_t y = g->height - 1; y < g->height; --y) {
            for (uint32_t x = 0; x < g->width; ++x) {
                assert(valid_coords(g, x, y));
                if (is_occupied(g, x, y))
                    sprintf(board + index++, "%u", get_owner(g, x, y) + 1);
                else
                    board[index++] = '.';
            }
            board[index++] = '\n';
        }
    }
    else {
        uint64_t size = (g->height) * (g->width + 1);
        board = malloc(size * sizeof(char) + 1);
        if (board == NULL)
            return false;
        for (uint32_t y = g->height - 1; y < g->height; --y) {
            for (uint32_t x = 0; x < g->width; ++x) {
                board[index++] = '[';
                if (is_occupied(g, x, y))
                    sprintf(board + index++, "%u", get_owner(g, x, y) + 1);
                else
                    board[index++] = '.';
                board[index++] = ']';
                if (index + 12 >= size) {
                    size *= 1.5;
                    board = realloc(board, size * sizeof(char) + 1);
                    if (errno == ENOMEM) {
                        free(board);
                        return NULL;
                    }
                }
            }
            board[index++] = '\n';
        }
    }

    board[index] = '\0';
    return board;
}
