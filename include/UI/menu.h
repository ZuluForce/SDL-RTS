#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>
#include "physics.h"
#include "actor_manager.h"
#include "screen_manager.h"
#include "std_gfx.h"
#include "event_manager.h"
#include "FastDelegate.h"
#include "FastDelegateBind.h"

using namespace fastdelegate;

class menu_button;
typedef FastDelegate1<int&,void> std_clbck;

class std_menu {
    private:
        SDL_Surface* default_static;
        SDL_Surface* default_hover;
        SDL_Surface* default_clicked;

        cID_dispatch option_ID;

        vector< menu_button* > menu;

    public:
        std_menu();
        ~std_menu();
        int new_menu_button(int x, int y, std_clbck callback);
        void set_background(SDL_Surface*);
        void set_button_image(SDL_Surface*);
        void set_b_image_hover(SDL_Surface*);
        void set_b_image_clicked(SDL_Surface*);
        void set_callback(int& button, std_clbck);
        void show_menu();
        void show_menu(int,int);
        void hide_menu();
        void hide_menu(int,int);
        friend void menu_input_events(SDL_Event*);
};

class menu_button: public cActor {
    private:
        sDisplay_info curr_info;
        vector<Uint8> _event_binds;

        SDL_Surface* std;
        SDL_Surface* hover;
        SDL_Surface* clicked;

        collision_zone click_box;

        std_clbck callback;
        std_fuse click_delay;

        bool click_state;
        bool hover_state;
    public:
        menu_button(int,int,SDL_Surface*,SDL_Surface*,SDL_Surface*);
        ~menu_button();

        int set_priority(int);
        void set_ID(int);
        void set_typeID(int);
        void reg_callback(std_clbck);

        vector<Uint8>* event_binds();

        bool check();
        void check_events(event_vector** events, int* load, Uint8* key_states);
        sDisplay_info* get_display();
        void show();
        void hide();
};

#endif // MENU_H_INCLUDED
