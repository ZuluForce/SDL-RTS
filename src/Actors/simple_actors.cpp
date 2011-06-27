Dot::Dot(int _typeID) {
    typeID = _typeID;
    update = true;

    sDisplay_info curr_info;
    curr_info.clip = NULL;
    curr_info.x = 0;
    curr_info.y = 0;
}

bool check() {
    return update;
}

bool Dot::set_image(char* filename) {
    curr_info.surf = load_image(filename);
}

void Dot::set_pos(int x, int y) {
    curr_info->x = x;
    curr_info->y = y;
    return;
}

int Dot::set_priority(int new_priority) {
    priority = new_priority;
    return new_priority;
}

sDisplay_info* Dot::get_display() {
    update = false;

    return curr_info;
}
