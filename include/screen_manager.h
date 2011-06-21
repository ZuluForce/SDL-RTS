#ifndef SCREEN_MANAGER_H_INCLUDED
#define SCREEN_MANAGER_H_INCLUDED

#include <cstdio>
#include <string>
#include "std_gfx.h"

using namespace std;


typedef class cScreen_manager {
    private:
        SDL_Surface* screen;
        bool visible;
        int maxFPS; //Initialized to 30
        int frame;

        std_timer* fps_timer;

        //Prevent SM desctructor from being called while auto-update thread is running

        Uint32 clr_to_uint(SDL_Color* color);

    public:
        bool SM_active_thread;
        SDL_Thread* SM_thread_ptr;

        cScreen_manager(int width, int height, int bpp, Uint32 flags, bool show);
        ~cScreen_manager();

        bool SM_set_caption(char* cap, char* icon = NULL);
        bool SM_show();
        bool SM_update();
        bool SM_set_bg(SDL_Color* fill_color = NULL, SDL_Surface* fill_image = NULL);
        bool SM_maxFPS(int max);
        bool SM_showFPS();
} cScreen_manager;

SDL_Thread* SM_start(cScreen_manager* SM);
int start_SM_thread(void* SM);

#endif // SCREEN_MANAGER_H_INCLUDED
