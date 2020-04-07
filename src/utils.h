#ifndef GAMMA_UTILS_H
#define GAMMA_UTILS_H

#include <stdbool.h>
#include <stdint.h>
#include "gamma.h"


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
                                direction_t direction);


inline bool valid_player(gamma_t *g, uint32_t player);


inline field_t* get_field_ptr(gamma_t *g, uint32_t x, uint32_t y);


inline uint32_t is_occupied(gamma_t *g, uint32_t x, uint32_t y);


inline uint32_t get_owner(gamma_t *g, uint32_t x, uint32_t y);


inline void set_owner(gamma_t *g, uint32_t player,
                      uint32_t x, uint32_t y);


bool belongs_to_player(gamma_t *g, uint32_t player, uint32_t x,
                       uint32_t y, direction_t direction);


bool has_neighbour_of_player(gamma_t *g, uint32_t player,
                             uint32_t x, uint32_t y);


inline void set_parent(field_t *parent_ptr, field_t *child_ptr);


inline void set_parent_by_coords(gamma_t *g, field_t *parent_ptr,
                                 uint32_t x, uint32_t y);


inline void compress_path(void *parent_ptr, void *child_ptr);


field_t* find_representative(field_t *field_ptr);


void unite_areas(gamma_t *g, uint32_t x1, uint32_t y1,
                 uint32_t x2, uint32_t y2);


inline field_t* get_representative(gamma_t *g, uint32_t x, uint32_t y);


inline bool visited(gamma_t *g, uint32_t x, uint32_t y);


inline void set_visited(gamma_t *g, bool flag, uint32_t x, uint32_t y);


bool start_bfs(gamma_t *g, bfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y, direction_t direction);


#endif //GAMMA_UTILS_H
