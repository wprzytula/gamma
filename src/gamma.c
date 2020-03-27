

#include <stdlib.h>
#include "gamma.h"


typedef struct {
    uint32_t occupied_by;

} field_t;


typedef struct {
    uint32_t width;
    field_t *tiles;
} row_t;


typedef struct {
    uint32_t height;
    row_t *rows;
} board_t;


typedef struct {
    bool golden_move_available;
    uint32_t occupied_areas;
} player;


struct gamma {
    uint32_t height;
    uint32_t width;
    uint32_t players;
    uint32_t areas;
    board_t board;


};

gamma_t* gamma_new(uint32_t width, uint32_t height,
                   uint32_t players, uint32_t areas) {
    return NULL;
};


void gamma_delete(gamma_t *g);


bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);


bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y);


uint64_t gamma_busy_fields(gamma_t *g, uint32_t player);


uint64_t gamma_free_fields(gamma_t *g, uint32_t player);


bool gamma_golden_possible(gamma_t *g, uint32_t player);


char* gamma_board(gamma_t *g);