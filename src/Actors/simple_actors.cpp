#include "Actors/simple_actors.h"

extern cPhysic_manager* pPM;
extern cActor_manager* pAM;

Dot::Dot(int _typeID) {
    typeID = _typeID;
    update = true;
    wasd = false;

    //sDisplay_info curr_info;
    curr_info.x = 0;
    curr_info.y = 0;
    curr_info.clip = NULL;
    curr_info.surf = NULL;

    move_speed = 2;
    //pressed_key = {-1,-1};

    _event_binds = vector<Uint8>(1,SDL_KEYDOWN);
    _event_binds.push_back(SDL_KEYUP);

    p_container = new phys_cont;
    p_container->obj_info = &curr_info;
    p_container->init();

    p_container->actorType = _typeID;
    p_container->param.w_h->first = 20;
    p_container->param.w_h->second = 20;
    //p_container->gravity = 1;
    pPM->PM_register_collision_obj(p_container);
    return;
}

bool Dot::check() {
    return update;
}

void Dot::check_events(event_vector** events, int* load, Uint8* key_states) {
    event_vector* key_events = events[SDL_KEYDOWN];
    int i;
    SDL_Event key_event;

    if (wasd) {
        for (i = 0; i < load[SDL_KEYDOWN]; ++i) {
            key_event = key_events->at(i);
            switch( key_event.key.keysym.sym) {
                case SDLK_w:
                    pPM->PM_set_y_velocity(p_container,-move_speed);
                    update = true;
                    pressed_key[0] = SDLK_UP;
                    break;
                case SDLK_s:
                    pPM->PM_set_y_velocity(p_container,move_speed);
                    update = true;
                    pressed_key[0] = SDLK_DOWN;
                    break;
                case SDLK_a:
                    pPM->PM_set_x_velocity(p_container,-move_speed);
                    update = true;
                    pressed_key[1] = SDLK_LEFT;
                    break;
                case SDLK_d:
                    pPM->PM_set_x_velocity(p_container,move_speed);
                    update = true;
                    pressed_key[1] = SDLK_RIGHT;
                    break;
                default:
                    break;
            }
        }

        key_events = events[SDL_KEYUP];
        for (i = 0; i < load[SDL_KEYUP]; ++i) {
            key_event = key_events->at(i);
            switch( key_event.key.keysym.sym) {
                case SDLK_w:
                    if (pressed_key[0] == SDLK_UP) {
                        if ( key_states[SDLK_s] ) {
                            pPM->PM_set_y_velocity(p_container,move_speed);
                            pressed_key[0] = SDLK_DOWN;
                        } else {
                            pPM->PM_set_y_velocity(p_container,0);
                            pressed_key[0] = -1;
                        }
                        update = true;
                    }
                    break;
                case SDLK_s:
                    if (pressed_key[0] == SDLK_DOWN) {
                        if ( key_states[SDLK_w] ) {
                            pPM->PM_set_y_velocity(p_container,-move_speed);
                            pressed_key[0] = SDLK_UP;
                        } else {
                            pPM->PM_set_y_velocity(p_container,0);
                            pressed_key[0] = -1;
                        }
                        update = true;
                    }
                    break;
                case SDLK_a:
                    if (pressed_key[1] == SDLK_LEFT) {
                        if ( key_states[SDLK_d] ) {
                            pPM->PM_set_x_velocity(p_container,move_speed);
                            pressed_key[1] = SDLK_RIGHT;
                        } else {
                            pPM->PM_set_x_velocity(p_container,0);
                            pressed_key[1] = -1;
                        }
                        update = true;
                    }
                    break;
                case SDLK_d:
                    if (pressed_key[1] == SDLK_RIGHT) {
                        if ( key_states[SDLK_a] ) {
                            pPM->PM_set_x_velocity(p_container,-move_speed);
                            pressed_key[1] = SDLK_LEFT;
                        } else {
                            pPM->PM_set_x_velocity(p_container,0);
                            pressed_key[1] = -1;
                        }
                        update = true;
                    }
                    break;
                default:
                    break;
            }
        }
    } else {
        for (i = 0; i < load[SDL_KEYDOWN]; ++i) {
            key_event = key_events->at(i);
            switch( key_event.key.keysym.sym) {
                case SDLK_UP:
                    pPM->PM_set_y_velocity(p_container,-move_speed);
                    update = true;
                    pressed_key[0] = SDLK_UP;
                    break;
                case SDLK_DOWN:
                    pPM->PM_set_y_velocity(p_container,move_speed);
                    update = true;
                    pressed_key[0] = SDLK_DOWN;
                    break;
                case SDLK_LEFT:
                    pPM->PM_set_x_velocity(p_container,-move_speed);
                    update = true;
                    pressed_key[1] = SDLK_LEFT;
                    break;
                case SDLK_RIGHT:
                    pPM->PM_set_x_velocity(p_container,move_speed);
                    update = true;
                    pressed_key[1] = SDLK_RIGHT;
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
                    if (pressed_key[0] == SDLK_UP) {
                        pPM->PM_set_y_velocity(p_container,0);
                        update = true;
                        pressed_key[0] = -1;
                    }
                    break;
                case SDLK_DOWN:
                    if (pressed_key[0] == SDLK_DOWN) {
                        pPM->PM_set_y_velocity(p_container,0);
                        update = true;
                        pressed_key[0] = -1;
                    }
                    break;
                case SDLK_LEFT:
                    if (pressed_key[1] == SDLK_LEFT) {
                        pPM->PM_set_x_velocity(p_container,0);
                        update = true;
                        pressed_key[1] = -1;
                    }
                    break;
                case SDLK_RIGHT:
                    if (pressed_key[1] == SDLK_RIGHT) {
                        pPM->PM_set_x_velocity(p_container,0);
                        update = true;
                        pressed_key[1] = -1;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    /* Make sure all collisions are checked and the obj is moved */
    pPM->PM_update(p_container);
    return;
}

bool Dot::set_image(char* filename) {
    curr_info.surf = load_image(filename);
    if ( curr_info.surf == NULL ) {
        fprintf(stderr,"Failed to set Dot image\n");
        return false;
    }
    return true;
}

bool Dot::set_image(SDL_Surface* image) {
    curr_info.surf = image;
    return true;
}

void Dot::set_pos(int x, int y) {
    curr_info.x = x;
    curr_info.y = y;

    pPM->PM_set_pos(p_container,x,y);
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
    return &curr_info;
}

SDL_Rect* Dot::get_clip() {
    return curr_info.clip;
}

void Dot::change_control() {
    wasd = wasd ? false : true;
    return;
}

static_obj::static_obj(int x, int y, SDL_Surface* surf) {
    curr_info.x = x;
    curr_info.y = y;
    curr_info.surf = surf;
    curr_info.clip = NULL;

    update = false;
}

void static_obj::move_to(int x, int y) {
    printf("Moving static obj to <%d,%d>\n",x,y);
    curr_info.x = x;
    curr_info.y = y;
}

void static_obj::move_delta(int x, int y) {
    curr_info.x += x;
    curr_info.y += y;
}

void static_obj::show() {
    update = true;
}

void static_obj::hide() {
    update = false;
}

char* static_obj::get_name() {
    return "Static Obj in \"Simple Actors\"";
}
