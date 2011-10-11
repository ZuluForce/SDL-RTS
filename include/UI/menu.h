#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <vector>
#include "menu_items.h"
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

        vector< menu_obj* > menu;

    public:
        std_menu();
        ~std_menu();
        int reg_menu_obj(menu_obj* new_obj, std_clbck callback);
        void set_background(SDL_Surface*);
        void set_button_image(SDL_Surface*);
        void set_b_image_hover(SDL_Surface*);
        void set_b_image_clicked(SDL_Surface*);
        void set_callback(int& button, std_clbck);
        void show_menu();
        void show_menu(int,int);
        void show_menu(int[]);
        void hide_menu();
        void hide_menu(int,int);
        void hide_menu(int[]);
        void set_back(int[]);
        int *get_menu_state();

        void dummy_callback(int &x) {
			return;
		}
        friend void menu_input_events(SDL_Event*);
};

#endif // MENU_H_INCLUDED
