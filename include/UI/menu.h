#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "physics.h"
#include "actor_manager.h"
#include "std_gfx.h"

class std_menu {
    private:
        SDL_Surface* default_static;
        SDL_Surface* default_active;

        int default_w,default_h;
        int screen_w,screen_h, w_center, h_center;
        int vertical_pad,horiz_pad;

        cID_dispatch option_ID;
        cID_dispatch menu_ID;

        vector< vector< menu_button* >> menus;

    public:
        std_menu();
        ~std_menu();
        int new_menu_button(int menuID, int x, int y);
        void set_background(SDL_Surface*);
        void set_button_image(SDL_Surface*);
        void set_b_image_active(SDL_Surface*);
        void set_vert_pad(int);
        void set_horiz_pad(int);
        friend void menu_input_events(SDL_Event*);
};

class menu_button: public cActor {
    private:
        sDisplay_info curr_info;
        vector<Uint8> _event_binds;

        SDL_Surface* std;
        SDL_Surface* active;
        SDL_Surface* clicked;

    public:
        menu_button(int,int,SDL_Surface*,SDL_Surface*,SDL_Surface*);
        ~menu_button();

        void set_ID(int);
        void set_typeID(int);

        vector<Uint8>* event_binds();
}

#endif // MENU_H_INCLUDED
