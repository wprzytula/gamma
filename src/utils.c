/** @file
 * Implementacja klasy zawierającej procedury pomocnicze gry gamma.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2.04.2020
 */

#include "utils.h"
#include "list.h"
#include "queue.h"

/**
 * Struktura przechowująca współrzędne pola na planszy.
 */
typedef struct {
    uint32_t x;  ///< numer kolumny
    uint32_t y;  ///< numer wiersza
} coords_t;


field_t *find_representative(field_t *field_ptr) {
    assert(field_ptr);
    list_t *trace = list_new();

    while (field_ptr->parent != field_ptr) {
        assert(field_ptr);
        list_append(trace, field_ptr);
        field_ptr = field_ptr->parent;
    }
    // path compression
    list_iter_kamikaze(compress_path, trace, field_ptr);

    return field_ptr;
}


void unite_areas(gamma_t *g, uint32_t x1, uint32_t y1,
                 uint32_t x2, uint32_t y2) {
    field_t *representative1 = find_representative(get_field_ptr(g, x1, y1));
    field_t *representative2 = find_representative(get_field_ptr(g, x2, y2));

    if (representative1 != representative2)
        set_parent(representative1, representative2);
}


bool start_bfs(gamma_t *g, bfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y) {
    assert(valid_player(g, player));
    if (!belongs_to_player(g, player, x, y))
        return false;
    if ((mode == TEST && visited(g, x, y)) ||
        (mode != TEST && !visited(g, x, y)))
        return false;

    field_t *new_representative = NULL;

    if (mode == UNITE) {
        new_representative = &g->board[y][x];
        assert(new_representative);
        set_parent(new_representative, new_representative);
    }

    uint32_t x_coords[4];
    uint32_t y_coords[4];

    queue_t *queue = queue_new();
    coords_t *coords = malloc(sizeof(coords_t));
    coords->x = x;
    coords->y = y;
    queue_push(queue, coords);

    while (!queue_is_empty(queue)) {
        coords = queue_pop(queue);
        x = coords->x;
        y = coords->y;
        free(coords);

        if ((visited(g, x, y) && mode == TEST) ||
            (!visited(g, x, y) && mode != TEST))
            continue;
        set_visited(g, mode == TEST, x, y);

        if (mode == UNITE)
            set_parent(new_representative, get_field_ptr(g, x, y));

        x_coords[0] = x     ; y_coords[0] = y + 1;
        x_coords[1] = x     ; y_coords[1] = y - 1;
        x_coords[2] = x + 1 ; y_coords[2] = y    ;
        x_coords[3] = x - 1 ; y_coords[3] = y    ;

        for (int i = 0; i < 4; ++i) {
            x = x_coords[i];
            y = y_coords[i];
            if (belongs_to_player(g, player, x, y) &&
                ((mode == TEST && !visited(g, x, y)) ||
                 (mode != TEST && visited(g, x, y)))) {
                coords = malloc(sizeof(coords_t));
                coords->x = x;
                coords->y = y;
                queue_push(queue, coords);
            }
        }
    }
    queue_delete(queue);
    return true;
}


int32_t calculate_new_owner_occ_areas_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]) {
    int32_t new_owner_occupied_areas_change = 1;
    list_t *adjacent_areas = list_new();
    field_t *parent;
    uint32_t x_;
    uint32_t y_;

    for (int i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (belongs_to_player(g, new_owner, x_, y_)) {
            parent = find_representative(get_field_ptr(g, x_, y_));
            if (!list_in(adjacent_areas, parent))
                --new_owner_occupied_areas_change;
            list_append(adjacent_areas, parent);
        }
    }
    list_delete(adjacent_areas);

    return new_owner_occupied_areas_change;
}

int32_t calculate_new_owner_adj_free_fields_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]) {
    int32_t new_owner_adj_fields_change = 0;
    uint32_t x_;
    uint32_t y_;

    for (int i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (valid_coords(g, x_, y_) && !is_occupied(g, x_, y_)
            && !has_neighbour_of_player(g, new_owner, x_, y_))
            ++new_owner_adj_fields_change;
    }
    return new_owner_adj_fields_change;
}


int32_t calculate_former_owner_adj_free_fields_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]) {
    int32_t former_owner_adj_fields_change = 0;
    uint32_t x_;
    uint32_t y_;

    for (int i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (valid_coords(g, x_, y_) && !is_occupied(g, x_, y_)
            && !has_neighbour_of_player(g, former_owner, x_, y_))
            --former_owner_adj_fields_change;
    }
    return former_owner_adj_fields_change;
}


int32_t calculate_former_owner_occ_areas_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]) {
    int32_t former_owner_occupied_areas_change = -1;

    for (int i = 0; i < 4; ++i) {
        if (start_bfs(g, TEST, former_owner,
                      x_coords[i], y_coords[i]))
            ++former_owner_occupied_areas_change;
    }

    return former_owner_occupied_areas_change;
}