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
        fprintf(stderr,"Could not build click box : surface was NULL\n");
    }
}

void build_click_box(int x, int y, int w, int h, collision_zone& click_box) {
    if ( w != -1 ) {
        click_box.param.w_h->first = w;
    } else {
        w = click_box.param.w_h->first;
    }

    if ( h != -1 ) {
        click_box.param.w_h->second = h;
    } else {
        w = click_box.param.w_h->second;
    }

    click_box.sides[0] = {x, y, x + w, y};
    click_box.sides[1] = {x + w, y, x + w, y + h};
    click_box.sides[2] = {x, y + h, x + w, y + h};
    click_box.sides[3] = {x, y, x, y + h};
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

void print_click_box(collision_zone& click_box) {
    #define SIDES side[0],side[1],side[2],side[3]
    int* side = click_box.sides[0];
    printf("Side 0:\n");
    printf("\t <%d,%d> ---> <%d,%d>\n",SIDES);

    side = click_box.sides[1];
    printf("Side 1:\n");
    printf("\t <%d,%d> ---> <%d,%d>\n",SIDES);

    side = click_box.sides[2];
    printf("Side 2:\n");
    printf("\t <%d,%d> ---> <%d,%d>\n",SIDES);

    side = click_box.sides[3];
    printf("Side 3:\n");
    printf("\t <%d,%d> ---> <%d,%d>\n\n",SIDES);
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

bool menu_obj::visible() {
	return update;
}

void menu_obj::reinit() {
    return;
}

/*------------------------------*/

/*----------- Menu Button -------------*/

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

void menu_button::reinit() {
    change_img(std);
}

back_button::back_button(int x, int y, surfp std, surfp hover, surfp clicked)
	: menu_button(x,y,std,hover,clicked)
{
	menu_list = NULL;
}

void back_button::set_menu_list(int *list, bool clear) {
	if ( clear && menu_list ) {
		free(menu_list);
	}
	menu_list = list;
	return;
}

void back_button::reg_callback(array_clbck func) {
	callback = func;
}

void back_button::call_clbck() {
	callback(menu_list);
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

void mute_button::reinit() {
    return;
}

menu_slider::menu_slider(int x, int y, surfp scale, surfp s_load, surfp slider) {
    this->scale = scale;
    this->s_load = s_load;
    this->slider = slider;

    horiz = true;
    s_bound = {x, x + scale->w};
    ret_vals = {0,0};
    ret_range = 0;

    slider_actor = new static_obj(x,y-2,slider);
    slider_actor->lock_on_y();
    load_bar = new static_obj(x,y,s_load);

    curr_info.x = x;
    curr_info.y = y;
    curr_info.surf = scale;
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
    load_bar->set_clip(&load_clip);

    update = false;
    click_state = false;
}

void menu_slider::set_slider_bound(int low, int high) {
    s_bound = {low,high};

    slider_actor->move_to(low,low);
}

void menu_slider::set_slider_pos(int percent) {
    int new_pos = (s_bound[1] - s_bound[0]) * percent;
    slider_actor->move_to(new_pos,new_pos);
}

void menu_slider::set_return_val(int low, int high) {
    ret_vals = {low, high};
    ret_range = high - low;
}

void menu_slider::move_slider(int& x, int& y, bool call_foo) {
    if ( horiz ) {
        slider_actor->move_to(x,y);
        move_load_horiz(x);

        if ( call_foo ) {
            //Difference between slider start and current position
            int diff = x - s_bound[0];

            float percent = ((float) diff) / (s_bound[1] - s_bound[0]);

            diff = (ret_vals[0] + ret_range * percent);
            callback( diff );
        }

    } else {
        slider_actor->move_to(x,y);
        move_load_vert(y);

        if ( call_foo ) {
            int diff = y - s_bound[0];

            float percent = ((float) diff) / (s_bound[1] - s_bound[0]);

            diff = ret_vals[0] + ret_range * percent;
            callback( diff );
        }
    }
}

void menu_slider::move_load_horiz(int x) {
    load_clip.w = x - load_clip.x;
}

void menu_slider::move_load_vert(int y) {
    load_clip.h = y - load_clip.y;
}

void menu_slider::check_events(event_vector** events, int* load, Uint8* key_states) {
    if ( !update ) {
        return;
    }

    coordinates xy;

    if ( click_state && load[SDL_MOUSEMOTION] != 0) {
        SDL_GetMouseState(&(xy.first),&(xy.second));

        if ( pPM->PM_check_point1(&click_box, &xy) ) {
            move_slider(xy.first,xy.second);
        }
    }

    SDL_Event event;
    for (int i = 0; i < load[SDL_MOUSEBUTTONDOWN]; ++i) {
        event = events[SDL_MOUSEBUTTONDOWN]->at(i);
        xy.first = event.button.x;
        xy.second = event.button.y;

        if ( event.button.button == SDL_BUTTON_LEFT &&
            pPM->PM_check_point1(&click_box, &xy) ) {
                /* Move Slider to new position */
                click_state = true;
                move_slider(xy.first,xy.second);
        }
    }

    for (int i = 0; i < load[SDL_MOUSEBUTTONUP]; ++i) {
        event = events[SDL_MOUSEBUTTONUP]->at(i);

        if ( event.button.button == SDL_BUTTON_LEFT ) {
            click_state = false;
        }
    }
}

void menu_slider::set_style(bool orient, Uint8 style) {
    //Horizontal
    if ( orient ) {
        slider_actor->lock_on_y();
        horiz = true;
    } else {
    //Vertical
        slider_actor->lock_on_x();
        horiz = false;
    }
    this->style = style;
}

void menu_slider::set_typeID(int _typeID) {
    typeID = _typeID;
}

/* Overridding functions for menu_slider */

int menu_slider::set_priority(int i) {
    priority = i;

    slider_actor->set_priority(i + 2);
    pAM->AM_register(slider_actor);
    load_bar->set_priority(i + 1);
    pAM->AM_register(load_bar);

    return i;
}

char* menu_slider::get_name() {
    return "Menu Scale";
}

void menu_slider::show() {
    update = true;
    slider_actor->show();
    load_bar->show();
}

void menu_slider::hide() {
    update = false;
    slider_actor->hide();
    load_bar->hide();
}

/* ------------------------------------- */
