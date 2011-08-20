#ifndef RUN_GAME_H_INCLUDED
#define RUN_GAME_H_INCLUDED

#include <cstdio>
#include "actor_manager.h"
#include "std_gfx.h"
#include "Actors/simple_actors.h"
#include "physics.h"
#include "UI/menu.h"

class cGame {
    private:
        SDL_Surface* G_dot_img;
        SDL_Surface* G_back;
        SDL_Surface* G_quit_button;
        SDL_Surface* G_quit_clicked;

        std_menu* main_menu;

        int menu_callback;

        bool game_thread_active;
        SDL_Thread* game_thread_ptr;
    public:
        cGame();
        ~cGame();
        void cleanup(int);
        void init_resources();
        void spawn_actor(void*);
        friend int start_menu(void*);
};

int start_menu(void*);


#endif // RUN_GAME_H_INCLUDED
