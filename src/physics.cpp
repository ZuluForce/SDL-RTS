#include "physics.h"

#define VECTOR_GRID_INIT_SIZE 10

cPhysic_manager::cPhysic_manager(int grid_width, int grid_height) {
    collision_zone_grid = (list<phys_rect*>**) malloc( grid_width * grid_height * sizeof(list<phys_rect*>*) );
    collision_obj_grid = (list<phys_rect*>**) malloc( grid_width * grid_height * sizeof(vector<phys_rect*>*) );

    for (int i = 0; i < grid_width; ++i) {
        for (int j = 0; j < grid_height; ++j) {
            collision_zone_grid = new list<phys_rect*>;
            collision_obj_grid = new list<phys_rect*>(VECTOR_GRID_INIT_SIZE);
            obj_grid_load[i][j] = 0;
        }
    }

    obj_id_manage = cID_dispatch();
    grid_w = grid_width;
    grid_h = grid_height;
    return;
}

void cPhysic_manager::PM_set_collide_zone(int x, int y, int width, int height, int level) {
    phys_rect* new_rect = new phys_rect;
    new_rect->x = x;
    new_rect->y = y;
    new_rect->width = width;
    new_rect->height = height;
    new_rect->level = level;
    collision_zone_grid[x / grid_w][y / grid_h];

    //Add across grid span
}

void cPhysic_manager::PM_register_collision_obj(phys_rect* obj) {
    int grid_x, grid_y;
    phys_rect->grid_locations = vector< pair<int,int> >;
    phys_rect->rectID = obj_id_manage.ID_getid();

    //Grid index
    grid_x = phys_rect->x / grid_w;
    grid_y = phys_rect->y / grid_h;

    printf("PM registered a phys_rect with ID: %d\n",phys_rect->rectID);
    printf("\nScreen Location: <%d,%d>\n",phys_rect->x,phys_rect->y);
    printf("\nGrid Location: <%d,%d>\n",grid_x,grid_y);

    phys_rect->grid_locations[0] = pair<int,int>(grid_x,grid_y);

    //How many grid sections an object spans
    x_span = phys_rect->width / grid_w;
    y_span = phys_rect->height / grid_h;

    if ( (phys_rect->x % grid_w) == 0 ) {
        for (int i = 1; (i < x_span) && (grid_x + i < grid_width); ++i) {
            collision_obj_grid[grid_x + i][grid_y]->push_back(obj);
            phys_rect->grid_locations.push_back( pair<int,int>(grid_x+i,grid_y) );
        }
    } else {
        for (int i = 0; (i < x_span) && (grid_x + i < grid_width); ++i) {
            collision_obj_grid[grid_x + i][grid_y]->push_back(obj);
            phys_rect->grid_locations.push_back( pair<int,int>(grid_x+i,grid_y) );
        }
    }

    if ( (phys_rect->y % grid_h) == 0) {
        for (int j = 1; (i < y_span) && (grid_y + i < grid_height); ++j) {
            collision_obj_grid[grid_x][grid_y + j]->push_back(obj);
            phys_rect->grid_locations.push_back( pair<int,int>(grid_x,grid_y+j) );
        }
    } else {
        for (int j = 0; (i < y_psan) && (grid_y + i < grid_height); ++ j) {
            collision_obj_grid[grid_x][grid_y + ]->push_back(obj);
            phys_rect->grid_locations.push_back( pair<int,int>(grid_x,grid_y+j) );
        }
    }

    collision_obj_grid[grid_x][grid_y]->push_back(obj);
}


phys_rect** cPhysic_manager::PM_merge_rects(phys_rect* rect1, phys_rect* rect2) {
    /* If I feel there is an efficiency concern then this will be implemented but
       for the time being it adds more complexity to the multi-threading and
       is a problem that the end developer should look out for. */
    //DO NOT MAKE SEPARATE OVERLAPPING/CONNECTED COLLISIONS ZONES
    ;
}

void cPhysic_manager::PM_move_obj(phys_rect* rect, int x, int y) {
    phys_rect* obj_rect;
    obj_rect = collision_obj_grid[rect->x][rect->y];
}
