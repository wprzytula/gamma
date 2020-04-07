#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include "list.h"
#include "queue.h"


bool valid_coords(gamma_t *g, uint32_t x, uint32_t y, direction_t direction) {
    if (   (y == 0 && direction == UP)
           || (y == 4294967295 && direction == DOWN )
           || (x == 0 && direction == RIGHT)
           || (x == 4294967295 && direction == LEFT ))
        return false;
    else
        return (x < g->width && y < g->height);
}


bool valid_player(gamma_t *g, uint32_t player) {
    return player < g->players_num;
}



field_t *get_field_ptr(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return &g->board[y][x];
}



uint32_t is_occupied(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].occupied;
}


uint32_t get_owner(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].occupied_by;
}



void set_owner(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    g->board[y][x].occupied = true;
    g->board[y][x].occupied_by = player;
}



bool belongs_to_player(gamma_t *g, uint32_t player, uint32_t x, uint32_t y,
                       direction_t direction) {
    return valid_coords(g, x, y, direction) && is_occupied(g, x, y)
           && get_owner(g, x, y) == player;
}



bool has_neighbour_of_player(gamma_t *g, uint32_t player,
                             uint32_t x, uint32_t y) {
    return (belongs_to_player(g, player, x, y - 1, DOWN) ||
            belongs_to_player(g, player, x, y + 1, UP)   ||
            belongs_to_player(g, player, x - 1, y, LEFT) ||
            belongs_to_player(g, player, x + 1, y, RIGHT));
}



void set_parent(field_t *parent_ptr, field_t *child_ptr) {
    assert(parent_ptr != NULL && child_ptr != NULL);
    child_ptr->parent = parent_ptr;
}




void set_parent_by_coords(gamma_t *g, field_t *parent_ptr,
                          uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    g->board[y][x].parent = parent_ptr;
}



void compress_path(void *parent_ptr, void *child_ptr) {
    set_parent(parent_ptr, (field_t*)child_ptr);
}


field_t *find_representative(field_t *field_ptr) {
    assert(field_ptr);

    // list for path compression
    list_t *trace = listNew(sizeof(field_ptr));

    while (field_ptr->parent != field_ptr) {
        assert(field_ptr);
        listAppend(trace, field_ptr);
        field_ptr = field_ptr->parent;
    }

    // path compression
    listIterKamikaze(compress_path, trace, field_ptr);

    return field_ptr;
}



void unite_areas(gamma_t *g, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) {
    field_t *representative1 = find_representative(get_field_ptr(g, x1, y1));
    field_t *representative2 = find_representative(get_field_ptr(g, x2, y2));

    if (representative1 != representative2)
        set_parent(representative1, representative2);
}



field_t *get_representative(gamma_t *g, uint32_t x, uint32_t y) {
    return find_representative(g->board[y][x].parent);
}


bool visited(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].already_visited;
}


void set_visited(gamma_t *g, bool flag, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    g->board[y][x].already_visited = flag;
}


bool start_bfs(gamma_t *g, bfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y, direction_t direction) {
    assert(valid_player(g, player));
    if (!valid_coords(g, x, y, direction))
        return false;
    if ((mode == TEST && visited(g, x, y)) ||
        (mode != TEST && !visited(g, x, y)))
        return false;

    field_t *parent_ptr = NULL;

    if (mode == SAVE) {
        parent_ptr = &g->board[y][x];
        assert(parent_ptr);
        set_parent(parent_ptr, parent_ptr);
    }

    uint32_t x_coords[4];
    uint32_t y_coords[4];

    queue_t *queue = queueNew(sizeof(coords_t*));
    coords_t *coords = malloc(sizeof(coords_t));
    coords->x = x;
    coords->y = y;
    queuePush(queue, coords);

    while (!queueIsEmpty(queue)) {
        coords = queuePop(queue);
        x = coords->x;
        y = coords->y;
        assert(valid_coords(g, x, y, FIXED));

        set_visited(g, mode == TEST, x, y);

        if (mode == SAVE)
            set_parent_by_coords(g, parent_ptr, x, y);

        x_coords[0] = x     ; y_coords[0] = y + 1;
        x_coords[1] = x     ; y_coords[1] = y - 1;
        x_coords[2] = x + 1 ; y_coords[2] = y    ;
        x_coords[3] = x - 1 ; y_coords[3] = y    ;

        for (int i = 0; i < 4; ++i) {
            x = x_coords[i];
            y = y_coords[i];

            if (belongs_to_player(g, player, x, y, i) &&
                ((mode == TEST && !visited(g, x, y)) ||
                 (mode != TEST && visited(g, x, y)))) {
                coords = malloc(sizeof(coords_t));
                coords->x = x;
                coords->y = y;
                queuePush(queue, coords);
            }
        }
    }
    queueDelete(queue);
    return true;
}
