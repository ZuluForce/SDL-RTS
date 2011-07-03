#include "Actors/simple_actors.h"

Dot::Dot(int _typeID) {
    typeID = _typeID;
    update = true;

    sDisplay_info curr_info;
    curr_info.clip = NULL;
    curr_info.x = 0;
    curr_info.y = 0;

    vector<int> _event_binds(1,SDL_KEYDOWN);
}

bool Dot::check() {
    return update;
}

void Dot::check_events(event_vector** events, int* load) {
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
