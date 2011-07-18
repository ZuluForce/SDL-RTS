#include "physics.h"

cPhysic_manager::cPhysic_manager(int grid_width, int grid_height) {
    collision_zone_grid = (phys_rect***) malloc( grid_width * grid_height * sizeof(phys_rect*) );
    return;
}

void cPhysic_manager::PM_set_collide_zone(int x, int y, int width, int height, int level) {
    phys_rect* new_rect = new phys_rect;
    new_rect->x = x;
    new_rect->y = y;
    new_rect->width = width;
    new_rect->height = height;
    new_rect->level = level;
    collision_zone_grid[x % grid_w][y % grid_h]
}


phys_rect** cPhysic_manager::PM_merge_rects(phys_rect* rect1, phys_rect* rect2) {
    /* If I feel there is an efficiency concern then this will be implemented but
       for the time being it adds more complexity to the multi-threading and
       is a problem that the end developer should look out for. */
    //DO NOT MAKE SEPARATE OVERLAPPING/CONNECTED COLLISIONS ZONES
    ;
}
