/** @file
 * Implementacja klasy przechowującej stan gry gamma
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 25.03.2020
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "gamma.h"
#include "list.h"
#include "utils.h"
#include <stdio.h>


gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas) {
    if (width == 0 || height == 0 || players == 0 || areas == 0
        || players >= width * height)
        return NULL;
    player_t *players_data = malloc(sizeof(player_t) * players);
    if (!players_data)
        return NULL;
    for (uint32_t i = 0; i < players; ++i) {
        players_data[i] = (player_t){.golden_move_available = true,
                                    .occupied_areas = 0,
                                    .taken_fields = 0,
                                    .available_adjacent_fields = 0};
    }
    board_t rows = malloc(sizeof(row_t) * height);
    if(!rows)
        return NULL;
    for (uint32_t i = 0; i < height; ++i) {
        rows[i] = malloc(sizeof(field_t) * width);
        if (!rows[i])
            return NULL;
        for (uint32_t j = 0; j < width; ++j) {
            rows[i][j] = (field_t){.already_visited = false,
                                   .occupied = false};
        }
    }
    gamma_t *gamma = malloc(sizeof(gamma_t));
    gamma->height = height;
    gamma->width = width;
    gamma->max_areas = areas;
    gamma->players_num = players;
    gamma->players = players_data;
    gamma->board = rows;

    return gamma;
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
    // okay, the move is available - go on

    ++g->players[player].taken_fields;
    ++g->players[player].occupied_areas;

    // init new area further unions
    field_t *this_field_ptr = get_field_ptr(g, x, y);
    set_parent(this_field_ptr, this_field_ptr);

    list_t *to_decrement = list_new();
    list_t *neigh_areas = list_new();

    uint32_t x_coords[4] = {x, x + 1, x, x - 1};
    uint32_t y_coords[4] = {y + 1, y, y - 1, y};

    uint32_t owner;
    player_t *owner_ptr;
    field_t *field_ptr;
    uint32_t x_;
    uint32_t y_;

    for (int i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (!valid_coords(g, x_, y_))
            continue;

        // decrement every neighbouring player free adjacent fields counter
        if (is_occupied(g, x_, y_)) {
            owner = get_owner(g, x_, y_);
            owner_ptr = &g->players[owner];
            if (!list_in(to_decrement, owner_ptr)) {
                list_append(to_decrement, owner_ptr);
                --g->players[owner].available_adjacent_fields;
            }
        }
        // calculate the change of current player free adjacent fields counter
        else if (!has_neighbour_of_player(g, player, x_, y_)) {
            ++g->players[player].available_adjacent_fields;
        }
        // find & union
        if (belongs_to_player(g, player, x_, y_)) {
            field_ptr = find_representative(get_field_ptr(g, x_, y_));
            if (!list_in(neigh_areas, field_ptr)) {
                list_append(neigh_areas, field_ptr);
                g->players[player].occupied_areas--;
            }
            unite_areas(g, x_, y_, x, y);
        }
    }
    list_delete(to_decrement);
    list_delete(neigh_areas);

    // actual move
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

    // now, when all initial prerequisites are met,
    // let's try to perform the actual *GOLDEN MOVE*

    uint32_t former_owner = get_owner(g, x, y);
    uint32_t new_owner = player;
    uint32_t x_coords[4] = {x, x + 1, x, x - 1};
    uint32_t y_coords[4] = {y + 1, y, y - 1, y};

    int32_t new_owner_adj_fields_change =
            calculate_new_owner_adj_free_fields_change(
                    g, new_owner, x_coords, y_coords);
    int32_t new_owner_occupied_areas_change =
            calculate_new_owner_occ_areas_change(
                    g, new_owner, x_coords, y_coords);

    // skipping linear time BFSes if the golden move is evidently impossible
    if (g->players[new_owner].occupied_areas + new_owner_occupied_areas_change
            > g->max_areas)
        goto golden_move_failed;

    // the trial golden move
    bool succeeded;
    set_owner(g, new_owner, x, y);
    set_visited(g, true, x, y);

    int32_t former_owner_adj_fields_change =
            calculate_former_owner_adj_free_fields_change(
                    g, former_owner, x_coords, y_coords);
    int32_t former_owner_occupied_areas_change =
            calculate_former_owner_occ_areas_change(
                    g, former_owner, x_coords, y_coords);

    // failed or not?
    if (g->players[former_owner].occupied_areas +
        former_owner_occupied_areas_change > g->max_areas) {
        golden_move_failed:
            set_owner(g, former_owner, x, y);
            succeeded = false;
    }
    else {
        g->players[new_owner].occupied_areas +=
                new_owner_occupied_areas_change;
        g->players[former_owner].occupied_areas +=
                former_owner_occupied_areas_change;
        g->players[new_owner].available_adjacent_fields +=
                new_owner_adj_fields_change;
        g->players[former_owner].available_adjacent_fields +=
                former_owner_adj_fields_change;
        ++g->players[new_owner].taken_fields;
        --g->players[former_owner].taken_fields;
        g->players[new_owner].golden_move_available = false;

        for (int i = 0; i < 4; ++i) {
            if (belongs_to_player(g, new_owner, x_coords[i], y_coords[i])) {
                unite_areas(g, x, y, x_coords[i], y_coords[i]);
            }
        }

        succeeded = true;
    }
    // clean up and, if succeeded, flush areas changes by uniting
    set_visited(g, false, x, y);
    for (int i = 0; i < 4; ++i) {
        start_bfs(g, succeeded ? UNITE : UNDO, former_owner,
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
        uint64_t result = g->width * g->height;
        for (uint32_t i = 0; i < g->players_num; ++i) {
            result -= g->players[i].taken_fields;
        }
        return result;
    }
    else {
        return g->players[player].available_adjacent_fields;
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
                }
            }
            board[index++] = '\n';
        }
    }

    board[index] = '\0';
    return board;
}
