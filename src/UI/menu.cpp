#include "UI\menu.h"

extern cActor_manager* pAM;
extern cPhysic_manager* pPM;

extern bool quit_threads;

std_menu::std_menu() {
    option_ID = cID_dispatch();

    default_w = default_h = -1;

    screen_w = pSM->SM_get_w();
    screen_h = pSM->SM_get_h();

    w_center = screen_w / 2;
    h_center = screen_h / 2;

    vertical_pad = horiz_pad = 0;
}

std_menu::~std_menu() {
    free(this.menus);
}

int std_menu::new_menu() {
    menus.push_back();
    return menu_ID.ID_getid();
}
int std_menu::new_menu_button(int menuID, int x, int y) {
    menu_button* new_button = new menu_button(x,y,default_static,default_active,NULL);
    new_button->set_ID(option_ID.ID_getid());
    menus[menuID].push_back(new_button);
    return option_ID.ID_recall();
}
void std_menu::set_background(SDL_Surface* img) {
    pAM->AM_set_bg(img);
    return;
}

void std_menu::set_button_image(SDL_Surface* img) {
    *default_static = *img;
    return;
}

void std_menu::set_b_image_active(SDL_Surface* img) {
    *default_active = *img;
    return;
}

void std_menu::show_menu(int meuID) {
    vector< menu_button* >::iterator button_it;

    for (button_it = menus[menuID].begin(); button_it != menus[menuID].end(); ++button_it) {
        (*button_it)->show();
    }

}

void std_menu::set_vert_pad(int pixels) {
    vertical_pad = pixels;
}

void std_menu::set_horiz_pad(int pixels) {
    horiz_pad = pixels;
}

menu_button::menu_button(int x, int y, SDL_Surface* std, SDL_Surface* active, SDL_Surface* clicked) {
    curr_info.x = x;
    curr_info.y = y;
    curr_info.surf = std;
    curr_info.clip = NULL;

    this.std = std;
    this.active = active;
    this.clicked = clicked;

    _event_binds.push_back(SDL_MOUSEBUTTONDOWN);
    _event_binds.push_back(SDL_MOUSEMOTION);

    update = true;
}

bool menu_button::check() {
    return update;
}

void menu_button::check_events(event_vector** events, int* load, Uint8* key_states) {
    ;
}

int set_priority(int i) {
    priority = i;
    return 0;
}

vector<Uint8>* menu_button::event_binds() {
    return &_event_binds;
}

sDisplay_info* menu_button::get_display()() {
    return &curr_info;
}

void menu_button::set_ID(int id) {
    ID = id;
}

void menu_button::set_typeID(int id) {
    typeID = id;
}

void menu_button::show() {

}
