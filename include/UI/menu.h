#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>
#include "physics.h"
#include "actor_manager.h"
#include "screen_manager.h"
#include "std_gfx.h"
#include "event_manager.h"

class menu_button;

class std_menu {
    private:
        SDL_Surface* default_static;
        SDL_Surface* default_active;
        SDL_Surface* default_clicked;

        int default_w,default_h;
        int screen_w,screen_h, w_center, h_center;
        int vertical_pad,horiz_pad;

        cID_dispatch option_ID;

        vector< menu_button* > menu;

    public:
        std_menu();
        ~std_menu();
        int new_menu_button(int x, int y);
        void set_background(SDL_Surface*);
        void set_button_image(SDL_Surface*);
        void set_b_image_active(SDL_Surface*);
        void set_b_image_clicked(SDL_Surface*);
        void set_vert_pad(int);
        void set_horiz_pad(int);
        void reg_callback(int button, void (*fn) (void*));
        void show_menu();
        friend void menu_input_events(SDL_Event*);
};

class menu_button: public cActor {
    private:
        sDisplay_info curr_info;
        vector<Uint8> _event_binds;

        SDL_Surface* std;
        SDL_Surface* active;
        SDL_Surface* clicked;

        collision_zone click_box;

        void (*callback) (void*);

    public:
        menu_button(int,int,SDL_Surface*,SDL_Surface*,SDL_Surface*);
        ~menu_button();

        int set_priority(int);
        void set_ID(int);
        void set_typeID(int);
        void reg_callback(void (*fn) (void*));

        vector<Uint8>* event_binds();

        bool check();
        void check_events(event_vector** events, int* load, Uint8* key_states);
        sDisplay_info* get_display();
        void show();
        void hide();
};

#endif // MENU_H_INCLUDED
