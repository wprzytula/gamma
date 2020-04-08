#include "utils.h"
#include "list.h"
#include "queue.h"


field_t *find_representative(field_t *field_ptr) {
    assert(field_ptr);
    list_t *trace = listNew();

    while (field_ptr->parent != field_ptr) {
        assert(field_ptr);
        listAppend(trace, field_ptr);
        field_ptr = field_ptr->parent;
    }
    // path compression
    listIterKamikaze(compress_path, trace, field_ptr);

    return field_ptr;
}


bool start_bfs(gamma_t *g, bfs_mode mode, uint32_t player,
               uint32_t x, uint32_t y, direction_t direction) {
    assert(valid_player(g, player));
    if (!belongs_to_player(g, player, x, y, direction))
        return false;
    if ((mode == TEST && visited(g, x, y)) ||
        (mode != TEST && !visited(g, x, y)))
        return false;

    field_t *new_representative = NULL;

    if (mode == SAVE) {
        new_representative = &g->board[y][x];
        assert(new_representative);
        set_parent(new_representative, new_representative);
    }

    uint32_t x_coords[4];
    uint32_t y_coords[4];

    queue_t *queue = queueNew();
    coords_t *coords = malloc(sizeof(coords_t));
    coords->x = x;
    coords->y = y;
    queuePush(queue, coords);

    while (!queueIsEmpty(queue)) {
        coords = queuePop(queue);
        x = coords->x;
        y = coords->y;
        free(coords);
        assert(valid_coords(g, x, y, FIXED));

        set_visited(g, mode == TEST, x, y);

        if (mode == SAVE)
            set_parent_by_coords(g, new_representative, x, y);

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
