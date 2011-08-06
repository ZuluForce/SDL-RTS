#include "Actors/simple_actors.h"

extern cPhysic_manager* pPM;
extern cActor_manager* pAM;

Dot::Dot(int _typeID) {
    typeID = _typeID;
    update = true;

    //sDisplay_info curr_info;
    curr_info = new sDisplay_info;
    curr_info->x = 0;
    curr_info->y = 0;
    curr_info->clip = NULL;
    curr_info->surf = NULL;

    move_speed = 2;
    //pressed_key = {-1,-1};

    _event_binds = vector<Uint8>(1,SDL_KEYDOWN);
    _event_binds.push_back(SDL_KEYUP);

    p_container = new phys_cont;
    p_container->obj_info = curr_info;
    p_container->init();

    p_container->actorType = _typeID;
    p_container->param.w_h->first = 20;
    p_container->param.w_h->second = 20;
    pPM->PM_register_collision_obj(p_container);
    return;
}

bool Dot::check() {
    return update;
}

void Dot::check_events(event_vector** events, int* load, Uint8* key_states) {
    /* Check if the Dot is already moving */
    /*
    for (int i = 0; i < 2; ++i) {
        switch ( pressed_key[i] ) {
            case SDLK_UP:
                if ( key_states[SDLK_UP] ) {
                    //curr_info->y -= move_speed;
                    pPM->PM_set_velocity(p_container,0, 0);
                    update = true;
                    break;
                }
            case SDLK_DOWN:
                if ( key_states[SDLK_DOWN] ) {
                    //curr_info->y += move_speed;
                    pPM->PM_set_velocity(p_container,0,0);
                    update = true;
                    break;
                }
            case SDLK_LEFT:
                if ( key_states[SDLK_LEFT] ) {
                    //curr_info->x -= move_speed;
                    pPM->PM_set_velocity(p_container,0,0);
                    update = true;
                    break;
                }
            case SDLK_RIGHT:
                if ( key_states[SDLK_RIGHT] ) {
                    //curr_info->x += move_speed;
                    pPM->PM_set_velocity(p_container,0,0);
                    update = true;
                    break;
                }
                default:
                    break;
        }
    } */

    event_vector* key_events = events[SDL_KEYDOWN];
    int i;
    SDL_Event key_event;
    for (i = 0; i < load[SDL_KEYDOWN]; ++i) {
        key_event = key_events->at(i);
        switch( key_event.key.keysym.sym) {
            case SDLK_UP:
                //curr_info->y -= move_speed;
                pPM->PM_set_y_velocity(p_container,move_speed);
                update = true;
                //pressed_key[0] = SDLK_UP;
                break;
            case SDLK_DOWN:
                //curr_info->y += move_speed;
                pPM->PM_set_y_velocity(p_container,-move_speed);
                update = true;
                //pressed_key[0] = SDLK_DOWN;
                break;
            case SDLK_LEFT:
                //curr_info->x -= move_speed;
                pPM->PM_set_x_velocity(p_container,-move_speed);
                update = true;
                //pressed_key[1] = SDLK_LEFT;
                break;
            case SDLK_RIGHT:
                //curr_info->x += move_speed;
                pPM->PM_set_x_velocity(p_container,move_speed);
                update = true;
                //pressed_key[1] = SDLK_RIGHT;
                break;
            default:
                break;
        }
    }

    key_events = events[SDL_KEYUP];
    for (i = 0; i < load[SDL_KEYUP]; ++i) {
        key_event = key_events->at(i);
        switch( key_event.key.keysym.sym) {
            case SDLK_UP:
                //curr_info->y += move_speed;
                pPM->PM_set_velocity(p_container,0,0);
                update = true;
                //if ( pressed_key[0] != SDLK_UP ) break;
                //pressed_key[0] = -1;
                break;
            case SDLK_DOWN:
                //curr_info->y -= move_speed;
                pPM->PM_set_velocity(p_container,0,0);
                update = true;
                //if ( pressed_key[0] != SDLK_DOWN ) break;
                //pressed_key[0] = -1;
                break;
            case SDLK_LEFT:
                //curr_info->x += move_speed;
                pPM->PM_set_velocity(p_container,0,0);
                update = true;
                //if ( pressed_key[1] != SDLK_LEFT ) break;
                //pressed_key[1] = -1;
                break;
            case SDLK_RIGHT:
                //curr_info->x -= move_speed;
                pPM->PM_set_velocity(p_container,0,0);
                update = true;
                //if ( pressed_key[1] != SDLK_RIGHT ) break;
                //pressed_key[1] = -1;
                break;
            default:
                break;
        }
    }

    /* Make sure all collisions are checked and the obj is moved */
    pPM->PM_update(p_container);
    return;
}

bool Dot::set_image(char* filename) {
    curr_info->surf = load_image(filename);
    if ( curr_info->surf == NULL ) {
        fprintf(stderr,"Failed to set Dot image\n");
        return false;
    }
    return true;
}

bool Dot::set_image(SDL_Surface* image) {
    curr_info->surf = image;
    return true;
}

void Dot::set_pos(int x, int y) {
    curr_info->x = x;
    curr_info->y = y;

    pPM->PM_move(p_container,x,y);
    return;
}

int Dot::set_priority(int new_priority) {
    priority = new_priority;
    return new_priority;
}

vector<Uint8>* Dot::event_binds() {
    return &_event_binds;
}

sDisplay_info* Dot::get_display() {
    update = false;
    //curr_info->clip = NULL;
    return curr_info;
}

SDL_Rect* Dot::get_clip() {
    return curr_info->clip;
}
