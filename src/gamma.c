/** @file
 * Implementacja klasy przechowującej stan gry gamma
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 25.03.2020
 */

#include <stdlib.h>
#include <stdint.h>
#include "gamma.h"
#include "list.h"
#include "queue.h"
#include "assert.h"



typedef struct {
    uint32_t x;
    uint32_t y;
} coords_t;


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



typedef enum {UP = 0, RIGHT= 1, DOWN = 2, LEFT = 3, FIXED = 4} direction_t;


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


static inline bool valid_coords(gamma_t *g, uint32_t x, uint32_t y,
                                direction_t direction) {
    if ((y == 0 && direction == UP)    || (y == 4294967295 && direction == DOWN ) ||
        (x == 0 && direction == RIGHT) || (x == 4294967295 && direction == LEFT ))
        return false;
    else
        return (x >= 0 && y >= 0 && x < g->width && y < g->height);
}


static inline bool valid_player(gamma_t *g, uint32_t player) {
    return player > 0 && player < g->players_num;
}


static inline field_t* get_field_ptr(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return &g->board[y][x];
}


static inline uint32_t is_occupied(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].occupied;
}


static inline uint32_t get_owner(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].occupied_by;
}


static inline void set_owner(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    g->board[y][x].occupied = true;
    g->board[y][x].occupied_by = player;
}


static inline field_t* get_parent(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].parent;
}


static inline void set_parent(field_t *parent_ptr, field_t *child_ptr) {
    assert(parent_ptr != NULL && child_ptr != NULL);
    child_ptr->parent = parent_ptr;
}

;
static inline void set_parent_by_coords(gamma_t *g, field_t *parent_ptr,
                              uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    g->board[y][x].parent = parent_ptr;
}


static bool belongs_to_player(gamma_t *g, uint32_t player, uint32_t x,
                              uint32_t y, direction_t direction) {
    if (valid_coords(g, x, y, direction) && is_occupied(g, x, y)
        && get_owner(g, x, y) == player)
        return true;
    else
        return false;
}


static bool has_neighbour_of_player(gamma_t *g, uint32_t player,
                                    uint32_t x, uint32_t y) {
    //assert(is_occupied(g, x, y)); <--- well, I should rethink this
    return (belongs_to_player(g, player, x, y - 1, DOWN) ||
            belongs_to_player(g, player, x, y + 1, UP)   ||
            belongs_to_player(g, player, x - 1, y, LEFT) ||
            belongs_to_player(g, player, x + 1, y, RIGHT));
}



static void compress_path(void *parent_ptr, void *child_ptr) {
    set_parent(parent_ptr, (field_t*)child_ptr);
}


static field_t* find_parent(field_t *field_ptr) {
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


static bool unite_areas(gamma_t *g, uint32_t x1, uint32_t y1,
                        uint32_t x2, uint32_t y2) {
    field_t *parent1 = find_parent(get_field_ptr(g, x1, y1));
    field_t *parent2 = find_parent(get_field_ptr(g, x2, y2));

    if (parent1 != parent2) {
        set_parent(parent1, parent2);
        return true;
    }
    else {
        return false;
    }
}


static bool
should_change_adj_field_count(gamma_t *g, uint32_t curr_player,
                              uint32_t x, uint32_t y, direction_t direction) {
    if (!valid_coords(g, x, y, direction) || !is_occupied(g, x, y)
        || get_owner(g, x, y) == curr_player)
        return false;
    else
        return true;
}

static void decrement_adj_field_count(void *ptr, void *player_ptr) {
    assert(!ptr);
    ((player_t*)player_ptr)->available_adjacent_fields--;
}


bool gamma_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL || !valid_coords(g, x, y, FIXED) || !valid_player(g, player))
        return false;
    if (g->board[y][x].occupied)
        return false;
    if (!has_neighbour_of_player(g, player, x, y) &&
        g->players[player].occupied_areas == g->max_areas)
        return false;
    // okay, the move is available - go on

    // start with incrementing occupied fields counter and changing field properties
    g->players[player].taken_fields++;

    // increment occupied areas counter as well
    g->players[player].occupied_areas++;

    // decrement every other neighbouring player free adjacent fields counter
    list_t *to_decrement = listNew(sizeof(uint32_t*));
    uint32_t x_coords[4] = {x, x + 1, x, x - 1};
    uint32_t y_coords[4] = {y + 1, y, y - 1, y};

    uint32_t owner;
    player_t *ownerPtr;
    for (direction_t i = UP; i <= LEFT; ++i) {
        if (should_change_adj_field_count(g, player, x_coords[i],
                                          y_coords[i], i)) {
            owner = get_owner(g, x_coords[i], y_coords[i]);
            ownerPtr = &g->players[owner];
            if (!listIn(to_decrement, ownerPtr))
                listAppend(to_decrement, ownerPtr);
        }
        // determine the change of current player free adjacent fields counter
        if (valid_coords(g, x_coords[i], y_coords[i], i)
            && !is_occupied(g, x_coords[i], y_coords[i])
            && !has_neighbour_of_player(g, player, x_coords[i],
                                        y_coords[i])) {
            g->players[player].available_adjacent_fields++;
        }
        // find & union
        if (unite_areas(g, x, y, x_coords[i], y_coords[i]))
            g->players[player].occupied_areas--;
    }
    listIterKamikaze(decrement_adj_field_count, to_decrement, NULL);

    return true;
}


static inline bool visited(gamma_t *g, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    return g->board[y][x].already_visited;
}


static inline void set_visited(gamma_t *g, bool flag, uint32_t x, uint32_t y) {
    assert(valid_coords(g, x, y, FIXED));
    g->board[y][x].already_visited = flag;
}


typedef enum {TEST = 0, UNDO = 1, SAVE = 2} bfs_mode;

static bool start_bfs(gamma_t *g, bfs_mode mode, uint32_t player,
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

            if (valid_coords(g, x, y, i) && get_owner(g, x, y) == player) {
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


bool gamma_golden_move(gamma_t *g, uint32_t player, uint32_t x, uint32_t y) {
    if (g == NULL || !valid_coords(g, x, y, FIXED) || !valid_player(g, player))
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

    list_t *adjacent_areas = listNew(sizeof(field_t*));

    uint32_t x_;
    uint32_t y_;

    for (int i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];

        // calculate new owner occupied areas change
        if (belongs_to_player(g, new_owner, x_, y_, i)) {
            field_t *parent = find_parent(get_parent(g, x_, y_));
            if (!listIn(adjacent_areas, parent))
                --new_owner_occupied_areas_change;
            listAppend(adjacent_areas, parent);
        }
        // calculate new owner adjacent free fields change
        if (valid_coords(g, x_, y_, i) && !is_occupied(g, x, y)
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

        if (valid_coords(g, x_, y_, i) && !is_occupied(g, x, y)
            && !has_neighbour_of_player(g, former_owner, x_, y_))
            --former_owner_adj_fields_change;
    }

    // calculate former owner occupied areas change
    for (int i = 0; i < 4; ++i) {
        if (start_bfs(g, TEST, former_owner,
                      x_coords[i], y_coords[i], i))
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
        succeeded = true;
    }
    set_visited(g, false, x, y);
    for (int i = 0; i < 4; ++i) {
        start_bfs(g, succeeded ? SAVE : UNDO, former_owner,
                  x_coords[i], y_coords[i], i);
    }
    return succeeded;
}


uint64_t gamma_busy_fields(gamma_t *g, uint32_t player) {
    if (g == NULL || !valid_player(g, player))
        return 0;
    else
        return g->players[player].taken_fields;
}


uint64_t gamma_free_fields(gamma_t *g, uint32_t player) {
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
    if (g != NULL && valid_player(g, player)
        && g->players[player].golden_move_available) {
        for (uint32_t i = 0; i < g->players_num; ++i) {
            if (g->players[i].taken_fields > 0 && i != player)
                return true;
        }
    }
    return false;
}


//char* gamma_board(gamma_t *g);