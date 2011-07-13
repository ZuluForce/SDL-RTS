#include "Actors/simple_actors.h"

Dot::Dot(int _typeID) {
    typeID = _typeID;
    update = true;

    sDisplay_info curr_info;
    curr_info.clip = NULL;
    curr_info.x = 0;
    curr_info.y = 0;

    move_speed = 2;
    pressed_key = {-1,-1};

    _event_binds = vector<Uint8>(1,SDL_KEYDOWN);
    _event_binds.push_back(SDL_KEYUP);
    return;
}

bool Dot::check() {
    return update;
}

void Dot::check_events(event_vector** events, int* load, Uint8* key_states) {
    /* Check if the Dot is already moving */
    for (int i = 0; i < 2; ++i) {
        switch ( pressed_key[i] ) {
            case SDLK_UP:
                if ( key_states[SDLK_UP] ) {
                    curr_info.y -= move_speed;
                    update = true;
                    break;
                }
            case SDLK_DOWN:
                if ( key_states[SDLK_DOWN] ) {
                    curr_info.y += move_speed;
                    update = true;
                    break;
                }
            case SDLK_LEFT:
                if ( key_states[SDLK_LEFT] ) {
                    curr_info.x -= move_speed;
                    update = true;
                    break;
                }
            case SDLK_RIGHT:
                if ( key_states[SDLK_RIGHT] ) {
                    curr_info.x += move_speed;
                    update = true;
                    break;
                }
                default:
                    break;
        }
    }

    event_vector* key_events = events[SDL_KEYDOWN];
    int i;
    SDL_Event key_event;
    for (i = 0; i < load[SDL_KEYDOWN]; ++i) {
        key_event = key_events->at(i);
        switch( key_event.key.keysym.sym) {
            case SDLK_UP:
                curr_info.y -= move_speed;
                update = true;
                pressed_key[0] = SDLK_UP;
                break;
            case SDLK_DOWN:
                curr_info.y += move_speed;
                update = true;
                pressed_key[0] = SDLK_DOWN;
                break;
            case SDLK_LEFT:
                curr_info.x -= move_speed;
                update = true;
                pressed_key[1] = SDLK_LEFT;
                break;
            case SDLK_RIGHT:
                curr_info.x += move_speed;
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
                curr_info.y += move_speed;
                update = true;
                if ( pressed_key[0] != SDLK_UP ) break;
                pressed_key[0] = -1;
                break;
            case SDLK_DOWN:
                curr_info.y -= move_speed;
                update = true;
                if ( pressed_key[0] != SDLK_DOWN ) break;
                pressed_key[0] = -1;
                break;
            case SDLK_LEFT:
                curr_info.x += move_speed;
                update = true;
                if ( pressed_key[1] != SDLK_LEFT ) break;
                pressed_key[1] = -1;
                break;
            case SDLK_RIGHT:
                curr_info.x -= move_speed;
                update = true;
                if ( pressed_key[1] != SDLK_RIGHT ) break;
                pressed_key[1] = -1;
                break;
            default:
                break;
        }
    }
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

void Dot::set_pos(int x, int y) {
    curr_info.x = x;
    curr_info.y = y;
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
