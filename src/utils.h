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


static inline bool valid_coords(gamma_t *g, uint32_t x, uint32_t y) {
    return (x < g->width && y < g->height);
}


static inline bool valid_player(gamma_t *g, uint32_t player) {
    return player < g->players_num;
}


static inline field_t *get_field_ptr(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return &g->board[y][x];
}

static inline uint32_t is_occupied(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return g->board[y][x].occupied;
}

static inline uint32_t get_owner(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return g->board[y][x].occupied_by;
}

static inline void set_owner(gamma_t *g, uint32_t player,
                             uint32_t x, uint32_t y) {
    g->board[y][x].occupied = true;
    g->board[y][x].occupied_by = player;
}

static inline bool belongs_to_player(gamma_t *g, uint32_t player,
                                     uint32_t x, uint32_t y) {
    return valid_coords(g, x, y) && is_occupied(g, x, y)
           && get_owner(g, x, y) == player;
}

static inline bool has_neighbour_of_player(gamma_t *g, uint32_t player,
                             uint32_t x, uint32_t y) {
    return (belongs_to_player(g, player, x, y + 1)
            || belongs_to_player(g, player, x + 1, y)
            || belongs_to_player(g, player, x, y - 1)
            || belongs_to_player(g, player, x - 1, y));
}

static inline void set_parent(field_t *parent_ptr, field_t *child_ptr) {
    assert(parent_ptr != NULL && child_ptr != NULL);
    child_ptr->parent = parent_ptr;
}

static inline void set_parent_by_coords(gamma_t *g, field_t *parent_ptr,
                          uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    g->board[y][x].parent = parent_ptr;
}

static inline void compress_path(void *parent_ptr, void *child_ptr) {
    set_parent(parent_ptr, (field_t*)child_ptr);
}

field_t* find_representative(field_t *field_ptr);


void unite_areas(gamma_t *g, uint32_t x1, uint32_t y1,
                 uint32_t x2, uint32_t y2);


static inline field_t *get_representative(gamma_t *g, uint32_t x, uint32_t y) {
    return find_representative(g->board[y][x].parent);
}

static inline bool visited(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    return g->board[y][x].already_visited;
}


static inline void set_visited(gamma_t *g, bool flag, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y));
    g->board[y][x].already_visited = flag;
}


bool start_bfs(gamma_t *g, bfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y);


int32_t calculate_new_owner_occupied_areas_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]);


int32_t calculate_new_owner_adj_free_fields_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]);


int32_t calculate_former_owner_adj_free_fields_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]);


int32_t calculate_former_owner_occupied_areas_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]);


#endif //GAMMA_UTILS_H
