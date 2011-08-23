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

int std_menu::new_menu_button(int x, int y, std_clbck callback) {
    menu_button* new_button = new menu_button(x,y,default_static,default_hover,default_clicked);
    new_button->set_ID(option_ID.ID_getid());
    new_button->set_priority(1);
    new_button->reg_callback(callback);
    menu.push_back(new_button);
    pAM->AM_register(new_button);
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
    vector< menu_button* >::iterator button_it;

    for (button_it = menu.begin(); button_it != menu.end(); ++button_it) {
        (*button_it)->show();
    }
}

void std_menu::show_menu(int start, int end) {
    for (int i = start; i <= end; ++i) {
        menu[i]->show();
    }
}

void std_menu::hide_menu() {
    vector< menu_button* >::iterator button_it;

    for (button_it = menu.begin(); button_it != menu.end(); ++button_it) {
        (*button_it)->hide();
    }
}

void std_menu::hide_menu(int start, int end) {
    for (int i = start; i <= end; ++i) {
        menu[i]->hide();
    }
}

menu_button::menu_button(int x, int y, SDL_Surface* std, SDL_Surface* hover, SDL_Surface* clicked) {
    curr_info.x = x;
    curr_info.y = y;
    curr_info.surf = std;
    curr_info.clip = NULL;

    this->std = std;
    this->hover = hover;
    this->clicked = clicked;

    _event_binds.push_back(SDL_MOUSEBUTTONDOWN);
    _event_binds.push_back(SDL_MOUSEBUTTONUP);
    _event_binds.push_back(SDL_MOUSEMOTION);

    click_box.x = x;
    click_box.y = y;
    click_box.param.w_h = new coordinates;
    if ( std != NULL ) {
        click_box.param.w_h->first = std->w;
        click_box.param.w_h->second = std->h;

        click_box.sides[0] = {x, y, x + std->w, y};
        click_box.sides[1] = {x + std->w, y, x + std->w, y + std->h};
        click_box.sides[2] = {x, y + std->h, x + std->w, y + std->h};
        click_box.sides[3] = {x, y, x, y + std->h};
    } else {
        click_box.param.w_h->first = 0;
        click_box.param.w_h->second = 0;

        click_box.sides[0] = {0,0,0,0};
        click_box.sides[1] = {0,0,0,0};
        click_box.sides[2] = {0,0,0,0};
        click_box.sides[3] = {0,0,0,0};
    }

    update = false;

    click_state = false;
    hover_state = false;
}

bool menu_button::check() {
    return update;
}

void menu_button::check_events(event_vector** events, int* load, Uint8* key_states) {
    if ( !update ) {
        return;
    }

    if (click_state && !click_delay.check()) {
        //curr_info.surf = clicked;
        click_state = false;
        callback(ID);
        return;
    }
    SDL_Event mouse_event;
    coordinates xy;
    for (int i = 0; i < load[SDL_MOUSEBUTTONDOWN]; ++i) {
        mouse_event = events[SDL_MOUSEBUTTONDOWN]->at(i);
        xy.first = mouse_event.button.x;
        xy.second = mouse_event.button.y;
        //printf("MouseButtonDown event at location <%d,%d>\n",xy.first,xy.second);
        if ( mouse_event.button.button == SDL_BUTTON_LEFT &&
            pPM->PM_check_point1(&click_box, &xy) ) {
                click_delay.start(100);
            //callback(ID);
            curr_info.surf = clicked;

            click_state = true;
        }
    }

    for (int i = 0; i < load[SDL_MOUSEBUTTONUP]; ++i) {
        mouse_event = events[SDL_MOUSEBUTTONUP]->at(i);
        if ( mouse_event.button.button == SDL_BUTTON_LEFT && click_state &&
            !click_delay.check()) {
            if ( hover_state ) {
                curr_info.surf = hover;
            } else {
                curr_info.surf = std;
            }
            click_state = false;
        }
    }
    if ( !click_state ) {
        for (int i = 0; i < load[SDL_MOUSEMOTION]; ++i) {
            mouse_event = events[SDL_MOUSEMOTION]->at(i);
            xy.first = mouse_event.motion.x;
            xy.second = mouse_event.motion.y;
            if ( pPM->PM_check_point1(&click_box, &xy) ) {
                hover_state = true;
                curr_info.surf = hover;
            } else {
                hover_state = false;
                curr_info.surf = std;
            }
        }
    }
}

int menu_button::set_priority(int i) {
    priority = i;
    return 0;
}

void menu_button::reg_callback(std_clbck func) {
    callback = func;
}

vector<Uint8>* menu_button::event_binds() {
    return &_event_binds;
}

sDisplay_info* menu_button::get_display() {
    if ( !update ) return NULL;
    return &curr_info;
}

void menu_button::set_ID(int id) {
    ID = id;
}

void menu_button::set_typeID(int id) {
    typeID = id;
}

void menu_button::show() {
    update = true;
}

void menu_button::hide() {
    update = false;
}
