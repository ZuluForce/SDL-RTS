#include "UI/menu.h"

extern cActor_manager* pAM;
extern cPhysic_manager* pPM;
extern cScreen_manager* pSM;

extern bool quit_threads;

std_menu::std_menu() {
    option_ID = cID_dispatch();
}

std_menu::~std_menu() {
    ;
}

int std_menu::reg_menu_obj(menu_obj* new_obj, std_clbck callback) {
	if ( !new_obj )
		return -1;

    new_obj->set_ID(option_ID.ID_getid());
    new_obj->set_priority(1);
    if ( callback )
		new_obj->reg_callback(callback);
    menu.push_back(new_obj);
    pAM->AM_register(new_obj);
    return option_ID.ID_recall();
}

void std_menu::set_background(SDL_Surface* img) {
    pAM->AM_set_bg(img);
    return;
}

void std_menu::set_button_image(SDL_Surface* img) {
    default_static = img;
    return;
}

void std_menu::set_b_image_hover(SDL_Surface* img) {
    default_hover = img;
    return;
}

void std_menu::set_b_image_clicked(SDL_Surface* img) {
    default_clicked = img;
    return;
}

void std_menu::set_callback(int& button, std_clbck func) {
    menu[button]->reg_callback(func);
}

void std_menu::show_menu() {
    vector< menu_obj* >::iterator obj_it;

    for (obj_it = menu.begin(); obj_it != menu.end(); ++obj_it) {
        (*obj_it)->show();
    }
}

void std_menu::show_menu(int start, int end) {
    for (int i = start; i <= end; ++i) {
        //printf("Menu is now showing object %d\n",i);
        menu[i]->show();
    }
}

void std_menu::show_menu(int list[]) {
	for (int i = 1; i < list[0]; ++i) {
		menu[list[i]]->show();
	}
}

void std_menu::hide_menu() {
    vector< menu_obj* >::iterator obj_it;

    for (obj_it = menu.begin(); obj_it != menu.end(); ++obj_it) {
        (*obj_it)->hide();
    }
}

void std_menu::hide_menu(int start, int end) {
    for (int i = start; i <= end; ++i) {
        menu[i]->hide();
    }
}

void std_menu::hide_menu(int list[]) {
	for (int i = 1; i < list[0]; ++i) {
		menu[list[i]]->hide();
	}
}

void std_menu::set_back(int list[]) {
	hide_menu();
	show_menu(list);
}

int *std_menu::get_menu_state() {
	int active_count = 0;
	vector< menu_obj* >::iterator obj_it;
	/* Count the number of active buttons */
	for (obj_it = menu.begin(); obj_it != menu.end(); ++obj_it) {
		if ( (*obj_it)->visible() )
			++active_count;
	}

	int *menu_state = (int*) malloc( sizeof(int) * (active_count + 1));
	menu_state[0] = active_count;

	int i = 1;
	int obj_pos = 0;
	for (obj_it = menu.begin(); obj_it != menu.end(); ++obj_it, ++obj_pos) {
		if ( (*obj_it)->visible() ) {
			menu_state[i] = obj_pos;
			++i;
		}
	}
	return menu_state;
}
