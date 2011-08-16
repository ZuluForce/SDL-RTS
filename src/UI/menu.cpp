#include "UI\menu.h"

extern cActor_manager* pAM;
extern cPhysic_manager* pPM;

std_menu::std_menu(int _typeID) {
    return;
}

void std_menu::set_background(SDL_Surface* img) {
    pAM->AM_set_bg(img);
    return;
}

void std_menu::set_button_image(SDL_Surface* img) {
    default_static = img;
    return;
}

void std_menu::set_b_image_active(SDL_Surface* img) {
    default_active = img;
    return;
}

void menu_input_events(SDL_Event* e) {

}

void std_menu() {
    return;
}
