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
    if (width == 0 || height == 0 || players == 0 || areas == 0)
        return NULL;
    player_t *players_data = malloc(sizeof(player_t) * players);
    for (uint32_t i = 0; i < players; ++i) {
        players_data[i] = (player_t){.golden_move_available = true,
                                    .occupied_areas = 0,
                                    .taken_fields = 0};
    }
    board_t rows = malloc(sizeof(row_t) * height);
    for (uint32_t i = 0; i < height; ++i) {
        rows[i] = malloc(sizeof(field_t) * width);
        for (uint32_t j = 0; j < width; ++j) {
            rows[i][j] = (field_t){.already_visited = false,
                                   .occupied = false,
                                   .parent = NULL};
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
    assert(player != 0);
    --player;
    if (g == NULL || !valid_coords(g, x, y)
        || !valid_player(g, player))
        return false;
    if (g->board[y][x].occupied)
        return false;
    if (g->players[player].occupied_areas == g->max_areas
        && !has_neighbour_of_player(g, player, x, y))
        return false;
    // okay, the move is available - go on

    ++g->players[player].taken_fields;
    ++g->players[player].occupied_areas;

    // init new area further unions
    field_t *this_field_ptr = &g->board[y][x];
    set_parent(this_field_ptr, this_field_ptr);

    list_t *to_decrement = listNew();
    list_t *neigh_areas = listNew();

    uint32_t x_coords[4] = {x, x + 1, x, x - 1};
    uint32_t y_coords[4] = {y + 1, y, y - 1, y};

    uint32_t owner;
    player_t *owner_ptr;
    field_t *field_ptr;
    uint32_t x_;
    uint32_t y_;

//    printf("Player %u moving into field %ux %uy\n", player + 1, x, y);

    for (int i = 0; i <= 3; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
//        printf("Testing coords: %ux %uy\n", x_, y_);
        if (!valid_coords(g, x_, y_))
            continue;

        // decrement every neighbouring player free adjacent fields counter
        if (is_occupied(g, x_, y_)) {
            owner = get_owner(g, x_, y_);
            owner_ptr = &g->players[owner];
            if (!listIn(to_decrement, owner_ptr)) {
                listAppend(to_decrement, owner_ptr);
//                printf("This field was adjacent to player %u\n", owner);
                --g->players[owner].available_adjacent_fields;
            }
        }
        // calculate the change of current player free adjacent fields counter
        else if (!has_neighbour_of_player(g, player, x_, y_)) {
//            printf("New free adj field: %ux %uy\n", x_, y_);
            ++g->players[player].available_adjacent_fields;
        }
        // find & union
        if (belongs_to_player(g, player, x_, y_)) {
            field_ptr = get_representative(g, x_, y_);
            if (!listIn(neigh_areas, field_ptr)) {
                listAppend(neigh_areas, field_ptr);
                --g->players[player].occupied_areas;
            }
            unite_areas(g, x_, y_, x, y);
        }
    }
    listDelete(to_decrement);
    listDelete(neigh_areas);

    // actual move
    set_owner(g, player, x, y);

    return true;
}


bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    assert(player != 0);
    --player;
    if (g == NULL || !valid_coords(g, x, y) || !valid_player(g, player))
        return false;
    if (!g->players[player].golden_move_available)
        return false;
    if (!is_occupied(g, x, y) || get_owner(g, x, y) == player)
        return false;
    if (g->players[player].occupied_areas == g->max_areas
        && !has_neighbour_of_player(g, player, x, y))
        return false;

    // now, when all initial prerequisites are met,
    // let's try to perform the actual *GOLDEN MOVE*

    // but first set up some counters and calculate some things
    uint32_t former_owner = get_owner(g, x, y);
    uint32_t new_owner = player;

    int32_t former_owner_adj_fields_change = 0;
    int32_t new_owner_adj_fields_change = 0;
    int32_t new_owner_occupied_areas_change = 1;
    int32_t former_owner_occupied_areas_change = -1;

    uint32_t x_coords[4] = {x, x + 1, x, x - 1};
    uint32_t y_coords[4] = {y + 1, y, y - 1, y};

    list_t *adjacent_areas = listNew();

    uint32_t x_;
    uint32_t y_;

    for (int i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];

        // calculate new owner occupied areas change
        if (belongs_to_player(g, new_owner, x_, y_)) {
            field_t *parent = find_representative(get_representative(g, x_, y_));
            if (!listIn(adjacent_areas, parent))
                --new_owner_occupied_areas_change;
            listAppend(adjacent_areas, parent);
        }
        // calculate new owner adjacent free fields change
        if (valid_coords(g, x_, y_) && !is_occupied(g, x_, y_)
            && !has_neighbour_of_player(g, new_owner, x_, y_))
            ++new_owner_adj_fields_change;
    }
    listDelete(adjacent_areas);

    // skipping linear time BFSes if the golden move is evidently impossible
    if (g->players[new_owner].occupied_areas + new_owner_occupied_areas_change
            > g->max_areas)
        goto golden_move_failed;

    // the trial golden move
    bool succeeded;
    set_owner(g, new_owner, x, y);
    set_visited(g, true, x, y);

    // calculate former owner adjacent free fields change
    for (int i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];

        if (valid_coords(g, x_, y_) && !is_occupied(g, x_, y_)
            && !has_neighbour_of_player(g, former_owner, x_, y_))
            --former_owner_adj_fields_change;
    }

    // calculate former owner occupied areas change
    for (int i = 0; i < 4; ++i) {
        if (start_bfs(g, TEST, former_owner,
                      x_coords[i], y_coords[i]))
            ++former_owner_occupied_areas_change;
    }

    // BFS going through every occupied field adjacent to the chosen one
    // toggling the visited flag on both player fields and adjacent free fields.

    // and finally, the work shall be done:
    // if any player exceeds the allowed number of occupied areas,
    // the whole golden move fails :(
    // we undo all the changes, using the second BFS.

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
        succeeded = true;
    }
    set_visited(g, false, x, y);
    for (int i = 0; i < 4; ++i) {
        start_bfs(g, succeeded ? SAVE : UNDO, former_owner,
                  x_coords[i], y_coords[i]);
    }
    return succeeded;
}


uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    assert(player != 0);
    --player;
    if (g == NULL || !valid_player(g, player))
        return 0;
    else
        return g->players[player].taken_fields;
}


uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
    assert(player != 0);
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
    assert(player != 0);
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
    char *board = malloc((g->height) * (g->width + 1) * sizeof(char) + 1);
    uint64_t index = 0;
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
    board[index] = '\0';
    return board;
}