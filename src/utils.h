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


typedef enum {UP = 0, RIGHT= 1, DOWN = 2, LEFT = 3, FIXED = 4} direction_t;


typedef struct {
    uint32_t x;
    uint32_t y;
} coords_t;


typedef enum {TEST = 0, UNDO = 1, SAVE = 2} bfs_mode;


inline bool valid_coords(gamma_t *g, uint32_t x, uint32_t y,
                         direction_t direction) {
    return !(y == 0 && direction == UP)
        && !(y == 4294967295 && direction == DOWN )
        && !(x == 0 && direction == RIGHT)
        && !(x == 4294967295 && direction == LEFT )
        && (x < g->width && y < g->height);
}


inline bool valid_player(gamma_t *g, uint32_t player) {
    return player < g->players_num;
}


inline field_t* get_field_ptr(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return &g->board[y][x];
}


inline uint32_t is_occupied(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].occupied;
}


inline uint32_t get_owner(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].occupied_by;
}


inline void set_owner(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    g->board[y][x].occupied = true;
    g->board[y][x].occupied_by = player;
}


inline bool belongs_to_player(gamma_t *g, uint32_t player, uint32_t x,
                              uint32_t y, direction_t direction) {
    return valid_coords(g, x, y, direction) && is_occupied(g, x, y)
           && get_owner(g, x, y) == player;
}


inline bool has_neighbour_of_player(gamma_t *g, uint32_t player,
                                    uint32_t x, uint32_t y) {
    return (belongs_to_player(g, player, x, y + 1, UP)
         || belongs_to_player(g, player, x + 1, y, RIGHT)
         || belongs_to_player(g, player, x, y - 1, DOWN)
         || belongs_to_player(g, player, x - 1, y, LEFT));
}


inline void set_parent(field_t *parent_ptr, field_t *child_ptr) {
    assert(parent_ptr != NULL && child_ptr != NULL);
    child_ptr->parent = parent_ptr;
}


inline void set_parent_by_coords(gamma_t *g, field_t *parent_ptr,
                                 uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    g->board[y][x].parent = parent_ptr;
}


inline void compress_path(void *parent_ptr, void *child_ptr) {
    set_parent(parent_ptr, (field_t*)child_ptr);
}


field_t* find_representative(field_t *field_ptr);


inline void unite_areas(gamma_t *g, uint32_t x1, uint32_t y1,
                        uint32_t x2, uint32_t y2) {
    field_t *representative1 = find_representative(get_field_ptr(g, x1, y1));
    field_t *representative2 = find_representative(get_field_ptr(g, x2, y2));

    if (representative1 != representative2)
        set_parent(representative1, representative2);
}


inline field_t* get_representative(gamma_t *g, uint32_t x, uint32_t y) {
    return find_representative(g->board[y][x].parent);
}


inline bool visited(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].already_visited;
}


inline void set_visited(gamma_t *g, bool flag, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    g->board[y][x].already_visited = flag;
}


bool start_bfs(gamma_t *g, bfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y, direction_t direction);


#endif //GAMMA_UTILS_H
