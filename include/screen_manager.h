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

        Uint32 clr_to_uint(SDL_Color* color);

    public:
        cScreen_manager(int width, int height, int bpp, Uint32 flags, bool show);

        /* May need to be changed to const char* */
        bool SM_set_caption(char* cap, char* icon = NULL);
        bool SM_show();
        bool SM_set_bg(SDL_Color* fill_color = NULL, SDL_Surface* fill_image = NULL);
} cScreen_manager;

#endif // SCREEN_MANAGER_H_INCLUDED
