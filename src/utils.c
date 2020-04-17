/** @file
 * Implementacja klasy zawierającej struktury, typy
 * i procedury pomocnicze gry gamma.
 * Procedury tu zdefiniowane przyjmują nieujemny numer gracza,
 * mniejszy o 1 od faktycznego.
 *
 * @author Wojciech Przytuła <wp418383@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2.04.2020
 */

#include "utils.h"


field_t *find_representative(field_t *field_ptr) {
    assert(field_ptr);
    field_t *trace[10];
    uint8_t quantity = 0;

    while (field_ptr->parent != field_ptr) {
        assert(field_ptr);
        if (quantity < 10)
            trace[quantity++] = field_ptr;
        field_ptr = field_ptr->parent;
    }
    for (uint8_t i = 0; i < quantity; ++i) {
        set_parent(field_ptr, trace[i]);
    }
    return field_ptr;
}


void unite_areas(gamma_t *g, uint32_t x1, uint32_t y1,
                 uint32_t x2, uint32_t y2) {
    field_t *representative1 = find_representative(get_field_ptr(g, x1, y1));
    field_t *representative2 = find_representative(get_field_ptr(g, x2, y2));

    if (representative1 != representative2)
        set_parent(representative1, representative2);
}


bool start_dfs(gamma_t *g, dfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y) {
    assert(valid_player(g, player));
    if (!belongs_to_player(g, player, x, y))
        return false;
    if (was_visited(g, mode, x, y))
        return false;

    field_t *new_representative = NULL;

    if (mode == UNITE) {
        new_representative = get_field_ptr(g, x, y);
        set_parent(new_representative, new_representative);
    }

    uint32_t x_coords[4];
    uint32_t y_coords[4];

    g->board[y][x].base_dir = RET;
    bool ret = true;
    direction_t direction = UP;
    uint32_t prev_x;
    set_visited(g, mode == TEST, x, y);

    do {
        x_coords[UP]    = x     ; y_coords[UP]    = y + 1;
        x_coords[RIGHT] = x + 1 ; y_coords[RIGHT] = y    ;
        x_coords[DOWN]  = x     ; y_coords[DOWN]  = y - 1;
        x_coords[LEFT]  = x - 1 ; y_coords[LEFT]  = y    ;

        if (!belongs_to_player(g, player, x, y) && ret)
            assert(false);
        if (!belongs_to_player(g, player, x, y)
        || (was_visited(g, mode, x, y) && !ret)) {
            x = x_coords[(direction + 2) % 4];
            y = y_coords[(direction + 2) % 4];
            ret = true;
            ++direction;
        }
        else if (!was_visited(g, mode, x, y) && ret) {
            assert(false);
        }
        else if (!ret) {
            // przygotowanie pola do cyklu
            set_visited(g, mode == TEST, x, y);
            g->board[y][x].base_dir = (direction + 2) % 4;
            if (mode == UNITE)
                set_parent(new_representative, get_field_ptr(g, x, y));
            direction = UP;
            ret = true;
        }
        else {
            if (direction == RET) {
                direction = (g->board[y][x].base_dir + 2) % 4 + 1;
                prev_x = x;
                x = x_coords[g->board[y][x].base_dir];
                y = y_coords[g->board[y][prev_x].base_dir];

            }
            else if (direction == g->board[y][x].base_dir) {
                ++direction;
            }
            else {
                x = x_coords[direction];
                y = y_coords[direction];
                ret = false;
            }
        }

    } while (!valid_coords(g, x, y) || direction < RET
             || g->board[y][x].base_dir < RET );
    return true;
}


int32_t calculate_player_adj_free_fields_change(gamma_t *g, bool count_for_new_owner,
        uint32_t player, uint32_t x_coords[], uint32_t y_coords[]) {
    int32_t player_adj_fields_change = 0;
    uint32_t x_;
    uint32_t y_;

    for (uint8_t i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (valid_coords(g, x_, y_) && !is_occupied(g, x_, y_)
            && !has_neighbour_of_player(g, player, x_, y_))
            ++player_adj_fields_change;
    }
    return count_for_new_owner ? player_adj_fields_change : -player_adj_fields_change;
}



int32_t calculate_new_owner_occ_areas_change(gamma_t *g,
        uint32_t new_owner, uint32_t x_coords[], uint32_t y_coords[]) {
    int32_t new_owner_occupied_areas_change = 1;
    field_t *adjacent_areas[4];
    uint8_t quantity = 0;
    bool already_considered;
    field_t *representative;
    uint32_t x_;
    uint32_t y_;

    for (uint8_t i = 0; i < 4; ++i) {
        x_ = x_coords[i];
        y_ = y_coords[i];
        if (belongs_to_player(g, new_owner, x_, y_)) {
            representative = find_representative(get_field_ptr(g, x_, y_));
            already_considered = false;
            for (uint8_t i = 0; i < quantity; ++i) {
                if (adjacent_areas[i] == representative) {
                    already_considered = true;
                    break;
                }
            }
            if (!already_considered)
                --new_owner_occupied_areas_change;
            adjacent_areas[quantity++] = representative;
        }
    }

    return new_owner_occupied_areas_change;
}


int32_t calculate_former_owner_occ_areas_change(gamma_t *g,
        uint32_t former_owner, uint32_t x_coords[], uint32_t y_coords[]) {
    int32_t former_owner_occupied_areas_change = -1;

    for (uint8_t i = 0; i < 4; ++i) {
        if (start_dfs(g, TEST, former_owner,
                      x_coords[i], y_coords[i]))
            ++former_owner_occupied_areas_change;
    }

    return former_owner_occupied_areas_change;
}