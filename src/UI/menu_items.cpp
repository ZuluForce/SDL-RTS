#include "UI/menu_items.h"

extern cPhysic_manager* pPM;
extern cActor_manager* pAM;
extern cAudio_manager* pAMM;

void build_click_box(int x, int y, SDL_Surface* std, collision_zone& click_box) {
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
}

void move_click_box(int dx, int dy, collision_zone& click_box) {
    int x = click_box.sides[0][0];
    int y = click_box.sides[0][1];
    int w = click_box.sides[0][2] - x;
    int h = click_box.sides[1][3] - y;

    click_box.sides[0] = {x + dx, y + dx, x + w + dx, y + dy};
    click_box.sides[1] = {x + w + dx, y + dy, x + w + dx, y + h + dy};
    click_box.sides[2] = {x + dx, y + h + dy, x + w + dx, y + h + dy};
    click_box.sides[3] = {x + dx, y + dy, x + dx, y + h + dy};
}

/* Default Declarations for menu_obj */

void menu_obj::call_clbck() {
    callback(ID);
}
void menu_obj::change_img(surfp img) {
    curr_info.surf = img;
}

void menu_obj::set_ID(int id) {
    ID = id;
}

void menu_obj::reg_callback(std_clbck func) {
    callback = func;
}

void menu_obj::show() {
    update = true;
}

void menu_obj::hide() {
    update = false;
}

/*------------------------------*/

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
    build_click_box(x,y,std,click_box);

    update = false;

    click_state = false;
    hover_state = false;
}

menu_button::~menu_button() {
    free( click_box.param.w_h );
}

bool menu_button::check() {
    return update;
}

void menu_button::call_clbck() {
    callback(ID);
}

void menu_button::change_img(surfp img) {
    curr_info.surf = img;
}

void menu_button::check_events(event_vector** events, int* load, Uint8* key_states) {
    if ( !update ) {
        return;
    }

    if (click_state && !click_delay.check()) {
        //curr_info.surf = clicked;
        click_state = false;
        call_clbck();
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
            //curr_info.surf = clicked;
            change_img(clicked);

            click_state = true;
        }
    }

    for (int i = 0; i < load[SDL_MOUSEBUTTONUP]; ++i) {
        mouse_event = events[SDL_MOUSEBUTTONUP]->at(i);
        if ( mouse_event.button.button == SDL_BUTTON_LEFT && click_state &&
            !click_delay.check()) {
            if ( hover_state ) {
                //curr_info.surf = hover;
                change_img(hover);
            } else {
                //curr_info.surf = std;
                change_img(std);
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
                //curr_info.surf = hover;
                change_img(hover);
            } else {
                hover_state = false;
                //curr_info.surf = std;
                change_img(std);
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

mute_button::mute_button(int x, int y, surfp std, surfp hover, surfp clicked)
    : menu_button(x,y,std,hover,clicked)
{
    muted = false;
    saved_vol = music_vol();
}

mute_button::~mute_button() {
        return;
}

void mute_button::call_clbck() {
    if ( muted ) {
        callback(saved_vol);
        muted = false;
    } else {
        saved_vol = music_vol();
        int new_vol = 0;
        callback(new_vol);
        muted = true;
    }
}

void mute_button::change_img(surfp img) {
    if ( muted ) {
        curr_info.surf = clicked;
    } else {
        curr_info.surf = std;
    }
}
int mute_button::music_vol() {
    return pAMM->AMM_get_music_vol();
}

menu_slider(int x, int y, surfp scale, surfp s_load, surfp slider) {
    this->scale = scale;
    this->s_load = s_load;
    this->slider = slider;

    slider_actor = static_obj(x,y,slider);

    curr_info.x = x;
    curr_info.y = y;
    curr_info.surf = slider;
    curr_info.clip = NULL;

    _event_binds.push_back(SDL_MOUSEBUTTONDOWN);
    _event_binds.push_back(SDL_MOUSEBUTTONUP);
    _event_binds.push_back(SDL_MOUSEMOTION);

    click_box.x = x;
    click_box.y = y;
    click_box.param.w_h = new coordinates;
    build_click_box(x,y,scale,click_box);

    load_clip.x = x;
    load_clip.y = y;
    load_clip.w = s_load->w;
    load_clip.h = s_load->h;

    update = false;
    click_state = false;
}

void menu_slider::set_slider_bound(int x, int x_high, int y, int y_high) {
    slide_bound = {x, x_high, y, y_high};

    /* Check if current slider position is in bound */
    /*
    curr_info.x = curr_info.x > x_high ? x_high : curr_info.x;
    curr_info.x = curr_info.x < x ? x : curr_info.x;
    curr_info.y = curr_info.y > y_high ? y_high : curr_info.y;
    curr_info.y = curr_info.y < y ? y : curr_info.y;
    */
    slider_actor.move_to(x,y);

    build_click_box(x,y,slider,click_box);

    load_clip.x = x;
    load_clip.y = y;
    load_clip.w = x_high - x;
    load_clip.h = y_high - y;
}

void menu_slider::check_events(event_vector** events, int* load, Uint8* key_states) {
    SDL_Event* event;
    coordinates xy;
    for (int i = 0; i < load[SDL_MOUSEBUTTONDOWN]; ++i) {
        event = events[SDL_MOUSEBUTTONDOWN]->at(i);
        xy.first = event.button.x;
        xy.second = event.button.y;

        if ( event.button.button == SDLK_LEFT &&
            pPM->PM_check_point1(&click_box, &xy) ) {
                /* Move Slider to new position */
                if ( horiz ) {
                    /* This works assuming you assign the slide bound
                    and rebuild the click_box for that bound */
                    curr_info.x = xy.first;
                } else {
                    curr_info.y = xy.second;
                }
        }
    }
}

void set_style(bool horiz, Uint8 style) {
    this->horiz = horiz;
    this->style = style;
}

void blit_load_bar(int load_percent) {
    if ( horiz ) {
        load_clip.w = (slide_bound[1] - slide_bound[0]) * load_percent;
    } else {
        load_clip.h = (slide_bound[3] - slide_bound[2]) * load_percent;
    }
}
