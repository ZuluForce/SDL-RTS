#include "Actors/simple_actors.h"

Dot::Dot(int _typeID) {
    typeID = _typeID;
    update = true;

    sDisplay_info curr_info;
    curr_info.clip = NULL;
    curr_info.x = 0;
    curr_info.y = 0;

    move_speed = 5;

    vector<int> _event_binds(1,SDL_KEYDOWN);
}

bool Dot::check() {
    return update;
}

void Dot::check_events(event_vector** events, int* load) {
    event_vector* key_events = events[SDL_KEYDOWN];
    int i;
    SDL_Event* key_event;
    for (i = 0; i < load[SDL_KEYDOWN]; ++i) {
        key_event = key_events->at(i);
        switch( key_event->key.keysym.sym) {
            case SDLK_UP:
                curr_info.y += move_speed;
                update = true;
                break;
            case SDLK_DOWN:
                curr_info.y -= move_speed;
                update = true;
                break;
            case SDLK_LEFT:
                curr_info.x -= move_speed;
                update = true;
                break;
            case SDLK_RIGHT:
                curr_info.x += move_speed;
                update = true;
                break;
            default:
                break;
        }
    }
    return;
}

bool Dot::set_image(char* filename) {
    curr_info.surf = load_image(filename);
    if ( curr_info.surf == NULL ) return false;
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

vector<SDL_EventType>* Dot::event_binds() {
    return &_event_binds;
}

sDisplay_info* Dot::get_display() {
    update = false;

    return &curr_info;
}
