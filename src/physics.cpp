#include "physics.h"

#define VECTOR_GRID_INIT_SIZE 10

extern cActor_manager* pAM;

void phys_cont::init(int _contType, int _level, int _actorType) {
    x = obj_info->x;
    y = obj_info->y;
    contType = _contType;
    level = _level;
    actorType = _actorType;

    coor_buffer = new coordinates;
    move_direction = new coordinates;

    switch ( contType ) {
        case 0:
            param.w_h = new coordinates;
            if ( !(obj_info->surf == NULL) ) {
                param.w_h->first = obj_info->surf->w;
                param.w_h->second = obj_info->surf->h;
            }
            break;
        case 1:
            param.radius = obj_info->surf->w;
            break;
        default:
            break;
    }
    return;
}

cPhysic_manager::cPhysic_manager(int grid_width, int grid_height) {
    collision_zone_grid = (list<phys_cont*>***) malloc(grid_height * sizeof(list<phys_cont*>**));
    collision_obj_grid = (list<phys_cont*>***) malloc(grid_height * sizeof(list<phys_cont*>**));
    obj_grid_load = (short**) malloc(grid_height * sizeof(short*));

    for (int i = 0; i < grid_height; ++i) {
        collision_zone_grid[i] = (list<phys_cont*>**) malloc( grid_width * sizeof(list<phys_cont*>*));
        collision_obj_grid[i] = (list<phys_cont*>**) malloc( grid_width * sizeof(list<phys_cont*>*));
        obj_grid_load[i] = (short*) calloc( grid_width, sizeof(short));
    }

    PM_init_grid(grid_width, grid_height);

    obj_id_manage = cID_dispatch();
    grid_w = grid_width;
    grid_h = grid_height;
    return;
}

void cPhysic_manager::PM_init_grid(int width, int height) {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            collision_obj_grid[i][j] = new list<phys_cont*>;
            collision_zone_grid[i][j] = new list<phys_cont*>;
        }
    }
}

void cPhysic_manager::PM_set_collide_zone(int x, int y, params parameters, int type, int level) {
    phys_cont* new_cont = new phys_cont;
    new_cont->x = x;
    new_cont->y = y;
    new_cont->param = parameters;
    new_cont->level = level;
    new_cont->contType = type;
    collision_zone_grid[x / grid_w][y / grid_h]->push_back(new_cont);
}

void cPhysic_manager::PM_register_collision_obj(phys_cont* obj) {

    int grid_x, grid_y;
    obj->grid_locations = new vector< pair<int,int> >;
    obj->PM_ID = obj_id_manage.ID_getid();

    //Grid index
    grid_x = obj->x / grid_w;
    grid_y = obj->y / grid_h;

    printf("\nPM registered a phys_rect with ID: %d\n",obj->PM_ID);
    printf("Screen Location: <%d,%d>\n",obj->x,obj->y);
    printf("Grid Location: <%d,%d>\n",grid_x,grid_y);

    obj->grid_locations->push_back( pair<int,int>(grid_x,grid_y) );


    switch ( obj->contType ) {
        //Rectangle
        case 0:
            PM_init_grid_loc_0(obj);
            break;
        default:
            break;
    }

    collision_obj_grid[grid_x][grid_y]->push_back(obj);
    return;
}

void cPhysic_manager::PM_check_collision(phys_cont* obj, bool shift) {
    switch ( obj->contType ) {
        case 0: //Rectangle
            PM_check_rect_(obj);
            break;
            /*
        case 1: //Circle
            PM_check_circle_(obj);
            break;
            */
        default:
            break;
    }
    if ( shift ) {
        printf("Moving object to <%d,%d>\n",obj->coor_buffer->first,obj->coor_buffer->second);
        obj->x = obj->coor_buffer->first;
        obj->y = obj->coor_buffer->second;

        obj->obj_info->x = obj->coor_buffer->first;
        obj->obj_info->y = obj->coor_buffer->second;
    }
    return;
}

void cPhysic_manager::PM_check_rect_(phys_cont* obj) {
    coordinates* saved_collision = new coordinates(0,0);
    coordinates* temp_collision = new coordinates(0,0);

    vector< pair<int,int> >::iterator coor_it;
    vector< pair<int,int> >* grid_vector = obj->grid_locations;

    list< phys_cont*>::iterator actor_it;
    list< phys_cont*>* cont_list;
    coordinates index;

    for (coor_it = grid_vector->begin(); coor_it != grid_vector->end(); ++coor_it) {
        index = *coor_it;
        cont_list = collision_obj_grid[index.first][index.second];
        for (actor_it = cont_list->begin(); actor_it != cont_list->end(); ++cont_list) {
            switch ( (*actor_it)->contType ) {
                case 0:
                    PM_check_rect_rect(obj, *actor_it);
                    //saved_collision = PM_resolve_collision(saved_collision, temp_collision);
                    break;
                    /*
                case 1:
                    temp_collision = PM_check_rect_circle(obj, *actor_it);
                    saved_collision = PM_resolve_collision(saved_collision, temp_collision);
                    break;*/
                default:
                    break;
            }
        }

        cont_list = collision_zone_grid[index.first][index.second];

        //Checking the collision "zones"
        for (actor_it = cont_list->begin(); actor_it != cont_list->end(); ++cont_list) {
            switch ( (*actor_it)->contType ) {
                case 0:
                    PM_check_rect_rect(obj, *actor_it);
                    //saved_collision = PM_resolve_collision(saved_collision, temp_collision);
                    break;
                    /*
                case 1:
                    temp_collision = PM_check_rect_circle(obj, *actor_it);
                    saved_collision = PM_resolve_collision(saved_collision, temp_collision);
                    break;
                    */
                default:
                    break;
            }
        }
    }
    //obj->coor_buffer = saved_collision;
    return;
}

/*
coordinates* cPhysic_manager::PM_check_circle_(phys_cont* obj) {
    return NULL;
}*/

void cPhysic_manager::PM_check_rect_rect(phys_cont* obj1, phys_cont* obj2) {
    //Check if any of the x-coordinates overlap
    if ( ((obj1->tx <= obj2->x) && (obj2->x <= (obj1->tx + obj1->param.w_h->first))) ||
        ((obj2->x <= obj1->tx) && (obj1->tx <= (obj2->x + obj2->param.w_h->first))) ) {

        //If so, check if any of the y-coordinates overlap
        if ( ((obj1->ty <= obj2->y) && (obj2->y <= (obj1->ty + obj1->param.w_h->second))) ||
            ((obj2->y <= obj1->ty) && (obj1->ty <= (obj2->y + obj2->param.w_h->second))) ) {
                obj1->coor_buffer->first = obj2->x - obj1->tx;
                obj2->coor_buffer->second = obj2->y - obj1->ty;
                obj1->x_vel = obj1->y_vel = obj1->x_accel = obj1->y_accel = 0;
                return;
        }
    }
    obj1->coor_buffer = NULL;
    return;
}

/*
void cPhysic_manager::PM_check_rect_circle(phys_cont* obj, phys_cont* obj2) {
    return NULL;
}

void cPhysic_manager::PM_check_circle_circle(phys_cont* obj, phys_cont* obj2) {
    return;
}*/

coordinates* cPhysic_manager::PM_resolve_collision(coordinates* coor1, coordinates* coor2) {
    if ( coor1 == NULL ) return coor2;
    if ( coor2 == NULL ) return coor1;
    return coor2;
    //return NULL;
}

void cPhysic_manager::PM_move(phys_cont* cont, int x, int y) {
    cont->x += x;
    cont->y += y;

    PM_check_collision(cont, true);

    cont->obj_info->x = x;
    cont->obj_info->y = y;
    return;
}

void cPhysic_manager::PM_update(phys_cont* cont) {
    cont->tx = cont->x + cont->x_vel;
    cont->ty = cont->y + cont->y_vel;

    PM_check_collision(cont, true);
    return;
}

void cPhysic_manager::PM_init_grid_loc_0(phys_cont* obj) {
    int grid_x = obj->x / grid_w;
    int grid_y = obj->y / grid_h;

    //int x_span = obj->param.w_h->first / grid_w;
    //int y_span = obj->param.w_h->second / grid_h;

    int x_span = 1;
    int y_span = 1;

    /* Adding all the spans across the x-axis */
    int i,j;

    if ( (obj->x % grid_w) == 0 ) {
        for (i = 1; (i < x_span) && (grid_x + i < grid_w); ++i) {
            collision_obj_grid[grid_x + i][grid_y]->push_back(obj);
            obj->grid_locations->push_back( pair<int,int>(grid_x+i,grid_y) );
        }
    } else {
        /* If the given object does not line directly up with a grid boundary it will span
         * an extra space */
        for (i = 0; (i < x_span) && (grid_x + i < grid_w); ++i) {
            collision_obj_grid[grid_x + i][grid_y]->push_back(obj);
            obj->grid_locations->push_back( pair<int,int>(grid_x+i,grid_y) );
        }
    }

    /* Adding all the spans across the y-axis */
    if ( (obj->y % grid_h) == 0) {
        for (j = 1; (i < y_span) && (grid_y + i < grid_h); ++j) {
            collision_obj_grid[grid_x][grid_y + j]->push_back(obj);
            obj->grid_locations->push_back( pair<int,int>(grid_x,grid_y+j) );
        }
    } else {
        for (j = 0; (i < y_span) && (grid_y + i < grid_h); ++ j) {
            collision_obj_grid[grid_x][grid_y + j]->push_back(obj);
            obj->grid_locations->push_back( pair<int,int>(grid_x,grid_y+j) );
        }
    }
    return;
}

void cPhysic_manager::PM_set_pos(phys_cont* cont, int x, int y) {
    cont->x = x;
    cont->y = y;

    cont->obj_info->x = x;
    cont->obj_info->y = y;
}

void cPhysic_manager::PM_set_velocity(phys_cont* cont, int x, int y) {
    cont->x_vel = x;
    cont->y_vel = y;
    return;
}

void cPhysic_manager::PM_set_x_velocity(phys_cont* cont, int x) {
    cont->x_vel = x;
    return;
}

void cPhysic_manager::PM_set_y_velocity(phys_cont* cont, int y) {
    cont->y_vel = y;
    return;
}
