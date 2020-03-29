/** @file
 * Implementacja klasy przechowującej stan gry gamma
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 25.03.2020
 */

#include <stdlib.h>
#include "gamma.h"
#include "list.h"
#include "queue.h"
#include "assert.h"


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
} player_t;


struct gamma {
    uint32_t height;
    uint32_t width;
    uint32_t players_num;
    uint32_t max_areas;
    player_t *players;
    board_t board;
};


gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas) {
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
}


static bool valid_coords(gamma_t *g, uint32_t x, uint32_t y) {
    return (x >= 0 && y >= 0 && x < g->width && y < g->height);
}


static bool
belongs_to_given_player(gamma_t *g, uint32_t player,
                                    uint32_t x, uint32_t y) {
    if (valid_coords(g, x, y) && g->board[y][x].occupied &&
        g->board[y][x].occupied_by == player)
        return true;
    else
        return false;
}


static bool
has_neighbour_of_given_player(gamma_t *g, uint32_t player,
                              uint32_t x, uint32_t y) {
    assert(g->board[y][x].occupied);
    return (belongs_to_given_player(g, player, x, y - 1) ||
            belongs_to_given_player(g, player, x, y + 1) ||
            belongs_to_given_player(g, player, x - 1, y) ||
            belongs_to_given_player(g, player, x + 1, y));

}


static field_t* find(field_t *field_ptr) {
    list_t *trace = listNew();
    while (field_ptr->parent != field_ptr) {
        listAppend(trace, );
    }
}


bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (!valid_coords(g, x, y))
        return false;
    if (g->board[y][x].occupied)
        return false;
    if (!has_neighbour_of_given_player(g, player, x, y) &&
        g->players[player].occupied_areas == g->max_areas)
        return false;

}


bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);


uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    if (g != NULL && player >= 0 && player < g->players_num)
        return g->players[player].taken_fields;
    else
        return 0;
}


uint64_t gamma_free_fields(gamma_t *g, uint32_t player);


bool gamma_golden_possible(gamma_t *g, uint32_t player) {
    if (g->players[player].golden_move_available) {
        for (uint32_t i = 0; i < g->players_num; ++i) {
            if (g->players[i].taken_fields > 0 && i != player)
                return true;
        }
    }
    return false;
}


char* gamma_board(gamma_t *g);