#include "physics.h"

#define VECTOR_GRID_INIT_SIZE 10
#define GRID_LOC(x) cont->grid_locations->at(x)
#define CONT_W cont->param.w_h->first
#define CONT_H cont->param.w_h->second

//For controlling debugging output
//#define _DEBUG 1
#define D_ZONE 1
#define D_LINE 1

extern cActor_manager* pAM;
extern cScreen_manager* pSM;

void phys_cont::init(int _contType, int _level, int _actorType) {
    x = obj_info->x;
    y = obj_info->y;
    tx = x;
    ty = y;
    x_vel = y_vel = 0;
    x_accel = y_accel = 0;

    contType = _contType;
    level = _level;
    actorType = _actorType;

    coor_buffer = new coordinates;
    coor_buffer->first = x;
    coor_buffer->second = y;
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

int cPhysic_manager::PM_set_collide_zone(int x, int y, params* _params, int _type) {
    collision_zone* new_zone = new collision_zone;
    new_zone->x = x;
    new_zone->y = y;
    new_zone->param = *_params;
    new_zone->contType = _type;
    new_zone->PM_ID = obj_id_manage.ID_getid();
    /* Incase coordinates are off the screen */
    x = PM_correct_x(x);
    y = PM_correct_y(y);

    new_zone->sides[0] = {x,y, x+ _params->w_h->first, y};
    new_zone->sides[1] = {x + _params->w_h->first, y, x + _params->w_h->first, y + _params->w_h->second};
    new_zone->sides[2] = {x, y + _params->w_h->second, x + _params->w_h->first, y + _params->w_h->second};
    new_zone->sides[3] = {x, y, x, y + _params->w_h->second};

    int grid_x,grid_y;
    grid_x = x / (screen_w / grid_w);
    grid_y = y / (screen_h / grid_h);

    int x_span,y_span,i,j;
    x_span = (new_zone->param.w_h->first + (new_zone->x % (screen_w / grid_w))) / (screen_w / grid_w);
    y_span = (new_zone->param.w_h->second + (new_zone->y % (screen_h / grid_h))) / (screen_h / grid_h);

    for (i = grid_x; i <= grid_x + x_span; ++i) {
        for (j = grid_y; j <= grid_y + y_span; ++j) {
            collision_zone_grid[i][j]->push_back(new_zone);
        }
    }
    //collision_zone_grid[x / (screen_w / grid_w)][y / (screen_h / grid_h)]->push_back(new_zone);
    printf("Collision zone created with coordinates: <%d,%d>\n",new_zone->x,new_zone->y);
    printf("\tCollision zone w/h: <%d,%d>\n\n",new_zone->param.w_h->first, new_zone->param.w_h->second);
    return new_zone->PM_ID;
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
        #ifdef _DEBUG
        printf("Moving object to <%d,%d>\n",obj->coor_buffer->first,obj->coor_buffer->second);
        #endif
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
    int most_recent = -1;
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
        for (zone_it = zone_list->begin(); zone_it != zone_list->end(); ++zone_it) {
            switch ( (*zone_it)->contType ) {
                case 0:
                    if ((*zone_it)->PM_ID == most_recent) continue;
                    PM_check_rect_zone(obj, *zone_it);
                    most_recent = (*zone_it)->PM_ID;
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
    return;
}

void cPhysic_manager::PM_check_rect_zone(phys_cont* cont, collision_zone* zone) {
    line coll_line;

    //Moving Down
    if ( cont->ty > cont->y ) {
        //Moving Down-Right
        if (cont->tx > cont->x) {
            //printf("Zone Check (Down-Right)\n");
            //Key corner
            coll_line = {cont->x + CONT_W, cont->y + CONT_H, cont->tx + CONT_W, cont->ty + CONT_H};

            #ifdef _DEBUG
            #ifdef D_ZONE
            printf("Down-Right:\n");
            printf("\tLine1 = ");
            PM_print_line(coll_line);
            printf("\tLine2 = ");
            PM_print_line(zone->sides[3]);
            printf("\n");
            printf("\tResult: %s\n\n",PM_check_lines(coll_line, zone->sides[3]) ? "True" : "False");
            #endif
            #endif

            if ( PM_check_lines(coll_line, zone->sides[0]) ) {

                #ifdef _DEBUG
                #ifdef D_ZONE
                printf("Zone Check (Down-Right) collides on side 0\n");
                printf("\tNew Pos = (%d,%d)\n",cont->tx,zone->y - CONT_H - 1);
                #endif
                #endif

                cont->coor_buffer->first = cont->tx;
                cont->coor_buffer->second = zone->y - CONT_H - 1;
                return;
            } else {
                if ( PM_check_lines(coll_line, zone->sides[3]) ) {
                    #ifdef _DEBUG
                    #ifdef D_ZONE
                    printf("Zone Check (Down-Right collides on side 4\n");
                    printf("\tNew Pos = (%d,%d)\n",zone->x - CONT_W - 1,cont->ty);
                    #endif
                    #endif
                    cont->coor_buffer->first = zone->x - CONT_W - 1;
                    cont->coor_buffer->second = cont->ty;
                    return;
                }
            }
            //Top-right corner
            coll_line = {cont->x + CONT_W, cont->y, cont->tx + CONT_W, cont->ty};
            if ( PM_check_lines(coll_line, zone->sides[3]) ) {
                    cont->coor_buffer->first = zone->x - CONT_W - 1;
                    cont->coor_buffer->second = cont->ty;
                    return;
            }

            //Bottom-Left corner
            coll_line = {cont->x, cont->y + CONT_H, cont->tx, cont->ty + CONT_H};
            if ( PM_check_lines(coll_line, zone->sides[0]) ) {
                cont->coor_buffer->first = cont->tx;
                cont->coor_buffer->second = zone->y - CONT_H - 1;
                return;
            }
        } else {
        //Moving Down-Left
            //Key Corner
            coll_line = {cont->x, cont->y + CONT_H, cont->tx, cont->ty + CONT_H};

            #ifdef _DEBUG
            #ifdef D_ZONE
            printf("Down-Left:\n");
            printf("\tLine1 = ");
            PM_print_line(coll_line);
            printf("\tLine2 = ");
            PM_print_line(zone->sides[0]);
            printf("\n");
            printf("\tResult: %s\n\n",PM_check_lines(coll_line, zone->sides[0]) ? "True" : "False");
            #endif
            #endif

            if ( PM_check_lines(coll_line, zone->sides[0]) ) {
                cont->coor_buffer->first = cont->tx;
                cont->coor_buffer->second = zone->y - CONT_H - 1;
                return;
            } else {
                if ( PM_check_lines(coll_line, zone->sides[1]) ) {
                    cont->coor_buffer->first = zone->x + zone->param.w_h->first + 1;
                    cont->coor_buffer->second = cont->ty;
                    return;
                }
            }
            //Top-Left corner
            coll_line = {cont->x, cont->y, cont->tx, cont->ty};
            if ( PM_check_lines(coll_line, zone->sides[1]) ) {
                    cont->coor_buffer->first = zone->x + zone->param.w_h->first + 1;
                    cont->coor_buffer->second = cont->ty;
                    return;
            }

            //Bottom-Right corner
            coll_line = {cont->x + CONT_H, cont->y + CONT_H, cont->tx + CONT_H, cont->ty + CONT_H};
            if ( PM_check_lines(coll_line, zone->sides[0]) ) {
                cont->coor_buffer->first = cont->tx;
                cont->coor_buffer->second = zone->y - CONT_H - 1;
                return;
            }
            return;
        }
    } else {
    //Moving Up
        //Moving Up-Right
        if (cont->tx > cont->x) {
            coll_line = {cont->x + CONT_H, cont->y, cont->tx + CONT_H, cont->ty};

            #ifdef _DEBUG
            #ifdef D_ZONE
            printf("Up-Right:\n");
            printf("\tLine1 = ");
            PM_print_line(coll_line);
            printf("\tLine2 = ");
            PM_print_line(zone->sides[3]);
            printf("\n");
            printf("\tResult: %s\n\n",PM_check_lines(coll_line, zone->sides[3]) ? "True" : "False");
            #endif
            #endif

            if ( PM_check_lines(coll_line, zone->sides[2]) ) {
                cont->coor_buffer->first = cont->tx;
                cont->coor_buffer->second = zone->y + zone->param.w_h->second + 1;
                return;
            } else {
                if ( PM_check_lines(coll_line, zone->sides[3]) ) {
                    cont->coor_buffer->first = zone->x - CONT_W - 1;
                    cont->coor_buffer->second = cont->ty;
                    return;
                }
            }
            //Top-Left corner
            coll_line = {cont->x, cont->y, cont->tx, cont->ty};
            if ( PM_check_lines(coll_line, zone->sides[2]) ) {
                    cont->coor_buffer->first = cont->tx;
                    cont->coor_buffer->second = zone->y + zone->param.w_h->second + 1;
                    return;
            }

            //Bottom-Right corner
            coll_line = {cont->x + CONT_H, cont->y + CONT_H, cont->tx + CONT_H, cont->ty + CONT_H};
            if ( PM_check_lines(coll_line, zone->sides[3]) ) {
                cont->coor_buffer->first = zone->x - CONT_W - 1;
                cont->coor_buffer->second = cont->ty;
                return;
            }
            return;
        } else {
        //Moving Up-Left
            coll_line = {cont->x, cont->y, cont->tx, cont->ty};

            #ifdef _DEBUG
            #ifdef D_ZONE
            printf("Up-Left:\n");
            printf("\tLine1 = ");
            PM_print_line(coll_line);
            printf("\tLine2 = ");
            PM_print_line(zone->sides[1]);
            printf("\n");
            printf("\tResult: %s\n\n",PM_check_lines(coll_line, zone->sides[1]) ? "True" : "False");
            #endif
            #endif

            if ( PM_check_lines(coll_line, zone->sides[1]) ) {
                cont->coor_buffer->first = zone->y + zone->param.w_h->first + 1;
                cont->coor_buffer->second = cont->ty;
                return;
            } else {
                if ( PM_check_lines(coll_line, zone->sides[2]) ) {
                    cont->coor_buffer->first = cont->tx;
                    cont->coor_buffer->second = zone->y + zone->param.w_h->second + 1;
                    return;
                }
            }
            //Top-Right corner
            coll_line = {cont->x + CONT_W, cont->y, cont->tx + CONT_W, cont->ty};
            if ( PM_check_lines(coll_line, zone->sides[2]) ) {
                    cont->coor_buffer->first = cont->tx;
                    cont->coor_buffer->second = zone->y + zone->param.w_h->second + 1;
                    return;
            }

            //Bottom-Left corner
            coll_line = {cont->x, cont->y + CONT_H, cont->tx, cont->ty + CONT_H};
            if ( PM_check_lines(coll_line, zone->sides[1]) ) {
                cont->coor_buffer->first = zone->x + zone->param.w_h->first + 1;
                cont->coor_buffer->second = cont->ty;
                return;
            }
            return;
        }
    }
    return;
}

/*
void cPhysic_manager::PM_check_rect_circle(phys_cont* obj, phys_cont* obj2) {
    return NULL;
}

void cPhysic_manager::PM_check_circle_circle(phys_cont* obj, phys_cont* obj2) {
    return;
}*/

bool cPhysic_manager::PM_check_lines(line line1, line line2) {
    float slope1,slope2;
    slope1 = line1[2] - line1[0];
    slope2 = line2[2] - line2[0];

    //Right-Hand-Side
    //int rhs;
    float rhs;
    //Checking for verticals
    if ( slope1 == 0 ) {
        //printf("\tLine1 is vertical\n");
        if ( slope2 == 0) {
            if ( line1[0] == line2[0]) {
                return true;
            }
            return false;
        } else {
            //printf("\tLine2 is not vertical\n");
            //line1 is vertical but not line2
            //Find line 2 slope
            slope2 = (line2[3] - line2[1]) / slope2;
            rhs = slope2 * (line1[0] - line2[0]) + line2[1];

            #ifdef _DEBUG
            #ifdef D_LINE
            printf("\tSlope2 = %f\n",slope2);
            printf("\tLines would intersect at x = %f\n",rhs);
            #endif
            #endif

            if ( ((line1[1] <= rhs && rhs <= line1[3]) ||
                (line1[3] <= rhs && rhs <= line1[1])) &&
                (line2[0] <= line1[0] && line1[0] <= line2[2]) ) {
                return true;
            } else {
                return false;
            }
        }
    } else {
        //Line2 is vertical but not line1
        //Find line1 slope
        if ( slope2 == 0) {
            slope1 = (line1[3] - line1[1]) / slope1;
            rhs = slope1 * (line2[0] - line1[0]) + line1[1];
            #ifdef _DEBUG
            #ifdef D_LINE
            printf("Line1 = ");
            PM_print_line(line1);
            printf("Line2 = ");
            PM_print_line(line2);
            printf("\n");

            printf("\tChecking if %d <= %.2f <= %d\n",line2[1],rhs,line2[3]);
            printf("\tAnd that %d <= %d <= %d\n",line1[0],line2[0],line1[2]);
            #endif
            #endif
            if ( line2[1] <= rhs && rhs <= line2[3] &&
                ((line1[0] <= line2[0] && line2[0] <= line1[2]) ||
                (line1[2] <= line2[0] && line2[0] <= line1[0]))) {
                    // The || is in-case line1[0] !< line1[2] which will often happen
                return true;
            } else {
                return false;
            }
        }
        slope1 = (line1[3] - line1[1]) / slope1;
        slope2 = (line2[3] - line2[1]) / slope2;
        if (slope1 == slope2) {
            //Parallel lines
            return false;
        }
        //Neither line is vertical
        //m1(x - x1) + y1 = m2(x - x2) + y2
        //Solving for x from two point-slope equations
        rhs = ( line2[1] - line1[1] + (slope2 * -line2[0]) - (slope1 * -line1[0]) ) / (slope1 - slope2);
        #ifdef _DEBUG
        #ifdef D_LINE
        printf("\tNeither line is vertical\n");
        printf("\tSlopes  slope1 = %.4f  slope2 = %.4f\n",slope1,slope2);
        printf("\tThe lines would intersect at x-coordinate : %.4f\n",rhs);
        #endif
        #endif
        if ( ((line1[0] <= rhs  && rhs <= line1[2]) ||
              (line1[2] <= rhs && rhs <= line1[0])) &&
            ((line2[0] <= rhs  && rhs <= line2[2]) ||
             (line2[2] <= rhs && rhs <= line2[0])) ) {
                return true;
            }
    }

return false;
}

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
    cont->ty = cont->y + cont->y_vel + cont->gravity;

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

    if ( cont->grid_locations->size() > 0) {
        for (i = 0; i < cont->grid_locations->size(); ++i) {
            #ifdef _DEBUG
            #ifdef D_ZONE
            printf("Removing cont from grid <%d,%d>\n",GRID_LOC(i).first,GRID_LOC(i).second);
            #endif
            #endif
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

    for (i = grid_x; i <= grid_x + x_span /*&& i < grid_w */; ++i) {
        for (j = grid_y; j <= grid_y + y_span /*&& j < grid_h */; ++j) {
            cont->grid_locations->push_back(coordinates(i,j));
            #ifdef _DEBUG
            #ifdef D_ZONE
            printf("Adding cont to grid <%d,%d>\n",i,j);
            #endif
            #endif

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

void cPhysic_manager::PM_print_line(line l) {
    printf("(%d,%d)->(%d,%d)\n",l[0],l[1],l[2],l[3]);
    return;
}

bool cPhysic_manager::PM_call_check(line l, line l2) {
    return PM_check_lines(l,l2);
}

bool cPhysic_manager::PM_check_point1(collision_zone* zone, coordinates* pt) {
    if ( ((zone->sides[0][0] <= pt->first) && (pt->first <= zone->sides[0][2])) &&
        ((zone->sides[1][1] <= pt->second) && (pt->second <= zone->sides[1][3])) ) {
            return true;
    }
    return false;
}

bool cPhysic_manager::PM_check_point2(phys_cont* cont, coordinates* point) {
    temp_line[0] = temp_line[2] = point->first;
    temp_line[1] = temp_line[3] = point->second;
    line cont_line;

    for (int i = 0; i < 4; ++i) {
        PM_create_line(cont,i,cont_line);
        if ( PM_check_lines( temp_line, cont_line) ) {
            return true;
        }
    }
    return false;
}

void cPhysic_manager::PM_create_line(phys_cont* cont, int side, line line_ptr) {
    switch( side ) {
        case 0:
            line_ptr[0] = cont->x;
            line_ptr[1] = line_ptr[3] = cont->y;
            line_ptr[2] = cont->x + CONT_W;
            break;
        case 1:
            line_ptr[0] = line_ptr[2] = cont->x + CONT_W;
            line_ptr[1] = cont->y;
            line_ptr[3] = cont->y + CONT_H;
            break;
        case 2:
            line_ptr[0] = cont->x;
            line_ptr[1] = line_ptr[3] = cont->y + CONT_H;
            line_ptr[2] = cont->x + CONT_W;
            break;
        case 3:
            line_ptr[0] = line_ptr[2] = cont->x;
            line_ptr[1] = cont->y;
            line_ptr[3] = cont->y + CONT_H;
            break;
        default:
            line_ptr[0] = line_ptr[2] = cont->x;
            line_ptr[1] = line_ptr[3] = cont->y;
            break;
    }
}
