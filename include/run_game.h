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
        SDL_Surface* G_menu_back;
        SDL_Surface* G_game_back;
        SDL_Surface* G_start_button;
        SDL_Surface* G_quit_button;
        SDL_Surface* G_quit_clicked;
        SDL_Surface* G_quit_hover;

        std_menu* main_menu;
        int b_start,b_quit;

        bool game_thread_active;
        SDL_Thread* game_thread_ptr;

        vector< cActor* > active_objs;
    public:
        cGame();
        ~cGame();
        void update();
        void cleanup(int);
        void init_resources();
        void spawn_actor(void*);
        void start_menu();
        /* Button Callbacks */
        void start_game(int&);
        void game_quit(int&);
        /*------------------*/

        friend int start_menu(void*);
};

int start_menu(void*);


#endif // RUN_GAME_H_INCLUDED
