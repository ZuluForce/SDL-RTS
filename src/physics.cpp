#include "physics.h"

#define VECTOR_GRID_INIT_SIZE 10
#define GRID_LOC(x) cont->grid_locations->at(x)

extern cActor_manager* pAM;
extern cScreen_manager* pSM;

void phys_cont::init(int _contType, int _level, int _actorType) {
    x = obj_info->x;
    y = obj_info->y;
    contType = _contType;
    level = _level;
    actorType = _actorType;

    coor_buffer = new coordinates;
    move_direction = new coordinates;
    grid_locations = new vector<coordinates>;

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

cPhysic_manager::cPhysic_manager(int grid_width, int grid_height, int screen_width, int screen_height) {
    collision_zone_grid = (list<collision_zone*>***) malloc(grid_height * sizeof(list<collision_zone*>**));
    collision_obj_grid = (list<phys_cont*>***) malloc(grid_height * sizeof(list<phys_cont*>**));

    for (int i = 0; i < grid_height; ++i) {
        collision_zone_grid[i] = (list<collision_zone*>**) malloc( grid_width * sizeof(list<collision_zone*>*));
        collision_obj_grid[i] = (list<phys_cont*>**) malloc( grid_width * sizeof(list<phys_cont*>*));
    }
    if ( screen_height == -1 || screen_width == -1) {
        screen_h = pSM->SM_get_h();
        screen_w = pSM->SM_get_w();
    } else {
        screen_h = screen_height;
        screen_w = screen_width;
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
            collision_zone_grid[i][j] = new list<collision_zone*>;
        }
    }
}

void cPhysic_manager::PM_set_collide_zone(int x, int y, params* _params, int _type) {
    collision_zone* new_zone = new collision_zone;
    new_zone->x = x;
    new_zone->y = y;
    new_zone->param = *_params;
    new_zone->contType = _type;
    new_zone->PM_ID = obj_id_manage.ID_getid();
    /* Incase coordinates are off the screen */
    x = PM_correct_x(x);
    y = PM_correct_y(y);
    collision_zone_grid[x / (screen_w / grid_w)][y / (screen_h / grid_h)]->push_back(new_zone);
}

void cPhysic_manager::PM_register_collision_obj(phys_cont* obj) {

    int grid_x, grid_y;
    obj->PM_ID = obj_id_manage.ID_getid();

    //Grid index
    grid_x = PM_correct_x(obj->x) / (screen_w / grid_w);
    grid_y = PM_correct_y(obj->y) / (screen_h / grid_h);

    //obj->grid_loc.first = grid_x;
    //obj->grid_loc.second = grid_y;
    //obj->grid_locations->push_back(coordinates(grid_x,grid_y));

    printf("\nPM registered a phys_rect with ID: %d\n",obj->PM_ID);
    printf("Screen Location: <%d,%d>\n",obj->x,obj->y);
    printf("Grid Location: <%d,%d>\n",grid_x,grid_y);

    //obj->grid_locations->push_back( pair<int,int>(grid_x,grid_y) );


    switch ( obj->contType ) {
        //Rectangle
        case 0:
            PM_reset_grid_loc(obj);
            break;
        default:
            break;
    }
    //Not needed if using PM_reset_grid_loc
    //collision_obj_grid[grid_x][grid_y]->push_back(obj);
    return;
}

void cPhysic_manager::PM_check_collision(phys_cont* obj, bool shift) {
    switch ( obj->contType ) {
        case 0: //Rectangle
            PM_check_rect_(obj);
            break;
        case 1: //Circle
            break;
        default:
            break;
    }
    if ( shift ) {
        //printf("Moving object to <%d,%d>\n",obj->coor_buffer->first,obj->coor_buffer->second);
        obj->x = obj->coor_buffer->first;
        obj->y = obj->coor_buffer->second;

        obj->obj_info->x = obj->coor_buffer->first;
        obj->obj_info->y = obj->coor_buffer->second;
        PM_reset_grid_loc(obj);
    }
    return;
}

void cPhysic_manager::PM_check_rect_(phys_cont* obj) {
    obj->coor_buffer->first = obj->tx;
    obj->coor_buffer->second = obj->ty;

    vector< pair<int,int> >::iterator coor_it;
    vector< pair<int,int> >* grid_vector = obj->grid_locations;
    list< phys_cont*>::iterator actor_it;
    list< collision_zone*>::iterator zone_it;

    list< phys_cont*>* cont_list;
    list< collision_zone*>* zone_list;

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
                case 1:
                    break;
                default:
                    break;
            }
        }

        zone_list = collision_zone_grid[index.first][index.second];
        //Checking the collision "zones"
        for (zone_it = zone_list->begin(); zone_it != zone_list->end(); ++zone_list) {
            switch ( (*zone_it)->contType ) {
                case 0:
                    PM_check_rect_zone(obj, *zone_it);
                    //saved_collision = PM_resolve_collision(saved_collision, temp_collision);
                    break;
                case 1:
                    break;
                default:
                    break;
            }
        }
    }
    return;
}

/*
coordinates* cPhysic_manager::PM_check_circle_(phys_cont* obj) {
    return NULL;
}*/

void cPhysic_manager::PM_check_rect_rect(phys_cont* obj1, phys_cont* obj2) {
    /*
    //Check if any of the x-coordinates overlap
    if ( ((obj1->tx <= obj2->x) && (obj2->x <= (obj1->tx + obj1->param.w_h->first))) ||
        ((obj2->x <= obj1->tx) && (obj1->tx <= (obj2->x + obj2->param.w_h->first))) ) {

        //If so, check if any of the y-coordinates overlap
        if ( ((obj1->ty <= obj2->y) && (obj2->y <= (obj1->ty + obj1->param.w_h->second))) ||
            ((obj2->y <= obj1->ty) && (obj1->ty <= (obj2->y + obj2->param.w_h->second))) ) {
                obj1->coor_buffer->first = obj2->x - obj1->tx;
                obj2->coor_buffer->second = obj2->y - obj1->ty;
                obj1->x_vel = obj1->y_vel = obj1->x_accel = obj1->y_accel = 0;
                //printf("There is a collision between rects\n");
                return;
        }
    } */
    //obj1->coor_buffer = NULL;
    obj1->coor_buffer->first = obj1->tx;
    obj1->coor_buffer->second = obj1->ty;
    return;
}

void cPhysic_manager::PM_check_rect_zone(phys_cont* cont, collision_zone* zone) {
    bool x_coll,y_coll;
    //Moving Right
    if ( cont->tx > cont->x ) {
        cont->coll_buffer[0].first = cont->x;
        cont->coll_buffer[0].second = cont->tx + cont->param.w_h->first;

        if ( cont->coll_buffer[0].first <= zone->x &&
            zone->x < cont->coll_buffer[0].second) {
            x_coll = true;
            cont->coll_buffer[0].first = zone->x;
        } else {
            x_coll = false;
            cont->coll_buffer[0].first = cont->coll_buffer[0].second;
        }
    } else {
        cont->coll_buffer[0].first = cont->x + cont->param.w_h->first;
        cont->coll_buffer[0].second = cont->tx;

        if ( cont->coll_buffer[0].first < (zone->x + zone->param.w_h->first) &&
            (zone->x + zone->param.w_h->first) <= cont->coll_buffer[0].second) {
            x_coll = true;
            cont->coll_buffer[0].first = zone->x + zone->param.w_h->first;
        } else {
            x_coll = false;
        }
    }

    //Moving Down
    if ( cont->ty > cont-> y ) {
        cont->coll_buffer[1].first = cont->y;
        cont->coll_buffer[1].second = cont->ty + cont->param.w_h->second;

        if ( cont->param.w_h->first <= zone->y &&
            zone->y < cont->param.w_h->second) {
            y_coll = true;
            cont->coll_buffer[1].first = zone->y;
        } else {
            y_coll = false;
            cont->coll_buffer[1].first = cont->coll_buffer[1].second;
        }
    } else {
        cont->coll_buffer[1].first = cont->y + cont->param.w_h->second;
        cont->coll_buffer[1].second = cont->ty;

        if ( cont->coll_buffer[1].second < (zone->y + cont->param.w_h->second) &&
            (zone->y + cont->param.w_h->second ) <= cont->coll_buffer[1].first ) {
            y_coll = true;
            cont->coll_buffer[1].first = zone->y + cont->param.w_h->second;
        } else {
            y_coll = false;
            /* Leave coll_buffer as is */
        }
    }
    /* In the case of a collision, the collision x-coordinate
       is saved in cont->coll_buffer[0].first and the
       y-coordinate is saved in cont->coll_buffer[1].first */
    cont->coor_buffer->first = cont->coll_buffer[0].first;
    cont->coor_buffer->second = cont->coll_buffer[1].first;
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
    //Should find were to set new_x and new_y
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

void cPhysic_manager::PM_init_grid_loc_0(phys_cont* cont) {
    return;
}

void cPhysic_manager::PM_reset_grid_loc(phys_cont* cont) {
    int i,j;
    int grid_x = PM_correct_x(cont->x) / (screen_w / grid_w);
    int grid_y = PM_correct_y(cont->y) / (screen_h / grid_h);
    int x_span,y_span;
    //Hasn't moved
    //if (grid_x == cont->grid_loc.first &&
    //    grid_y == cont->grid_loc.second) return;

    if ( cont->grid_locations->size() > 0) {
        for (i = 0; i < cont->grid_locations->size(); ++i) {
            PM_remove_from_grid(cont, GRID_LOC(i).first, GRID_LOC(i).second);
        }
    }

    switch ( cont->contType) {
        case 0:
            x_span = (cont->param.w_h->first + (cont->x % (screen_w / grid_w))) / (screen_w / grid_w);
            y_span = (cont->param.w_h->second + (cont->y % (screen_h / grid_h))) / (screen_h / grid_h);
            break;
        default:
            x_span = 0;
            y_span = 0;
            break;
    }

    cont->grid_locations->clear();

    for (i = grid_x; i <= grid_x + x_span; ++i) {
        for (j = grid_y; j <= grid_y + y_span; ++j) {
            cont->grid_locations->push_back(coordinates(i,j));
            PM_add_to_grid(cont,i,j);
        }
    }
    //PM_print_grid();
    return;
}

void cPhysic_manager::PM_set_pos(phys_cont* cont, int x, int y) {
    cont->new_x = cont->x != x ? true : false;
    cont->new_y = cont->y != y ? true : false;

    if (!cont->new_x && !cont->new_y) return;

    cont->x = x;
    cont->y = y;

    cont->obj_info->x = x;
    cont->obj_info->y = y;

    PM_reset_grid_loc(cont);
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

void cPhysic_manager::PM_set_gravity(phys_cont* cont, short gravity) {
    cont->gravity = gravity;
    return;
}

void cPhysic_manager::PM_remove_from_grid(phys_cont* cont, int x, int y) {
    if ( x >= grid_w || y >= grid_w ||
        x < 0 || y < 0) {
        return;
        }
    collision_obj_grid[x][y]->remove(cont);
    return;
}

void cPhysic_manager::PM_add_to_grid(phys_cont* cont, int x, int y) {
    if (x >= grid_w || y >= grid_w ||
        x < 0 || y < 0) {
        return;
        }
    collision_obj_grid[x][y]->push_back(cont);
    return;
}

int cPhysic_manager::PM_correct_x(int x) {
    x = x < 0 ? 0 : x;
    return x > screen_w ? screen_w : x;
}

int cPhysic_manager::PM_correct_y(int y) {
    y = y < 0 ? 0 : y;
    return y > screen_h ? screen_h : y;
}

void cPhysic_manager::PM_print_grid() {
    int i,j;
    for (j = 0; j < grid_h; ++j) {
        //Print top of row
        printf("---");
        for (i = 1; i < grid_w; ++i) {
            printf("--");
        }
        printf("\n");
        for (i = 0; i < grid_w; ++i) {
            printf("|%d",collision_obj_grid[i][j]->size());
        }
        printf("|\n");
    }
    printf("---");
    for (int i = 1; i < grid_w; ++i) {
        printf("--");
    }
    printf("\n\n");
}
