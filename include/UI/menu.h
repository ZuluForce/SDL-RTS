#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "physics.h"
#include "actor_manager.h"

class std_menu {
    private:
        SDL_Surface* default_static,default_active;

    public:
        std_menu();
        void set_background(SDL_Surface*);
        void set_button_image(SDL_Surface*);
        void set_b_image_active(SDL_Surface*);
        friend void menu_input_events(SDL_Event*);
};

void menu_input_events(SDL_Event*);

void std_menu();


#endif // MENU_H_INCLUDED
