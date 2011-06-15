#include "screen_manager.h"
#include "std_gfx.h"

using namespace std;

/* Class for controlling an SDL screen */

/* Converts SDL_Color to Uint32 */
Uint32 cScreen_manager::clr_to_uint(SDL_Color* color) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
Uint32 int_color = 0x00000000;
int_color += (color->r * 0x10000);
int_color += (color->g * 0x100);
int_color += color->b;
#else
Uint32 int_color = 0x00000000;
int_color += color->r;
int_color += (color->g * 0x100);
int_color += (color->b * 0x10000);
#endif

return int_color;
}

cScreen_manager::cScreen_manager(int width, int height, int bpp, Uint32 flags, bool show) {
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1) {
        fprintf(stderr, "SDL failed to initialize : %s on line %d\n", __FILE__, __LINE__);
        return;
    }

    screen = SDL_SetVideoMode(width, height, bpp, flags);

    if ( screen == NULL) {
        fprintf(stderr, "Failed to initialize the screen : %s on line %d\n",__FILE__,__LINE__);
        return;
    }

    if ( show ) SDL_Flip(screen);
    maxFPS = 30;
    return;
}

bool cScreen_manager::SM_set_caption(char* cap, char* icon) {
    if (icon == NULL) {
        SDL_WM_SetCaption(cap, cap);
    } else SDL_WM_SetCaption(cap, icon);

    return true;
}

bool cScreen_manager::SM_show() {
    if (SDL_Flip(screen) == -1) {
        fprintf(stderr, "Failed to flip the screen: %s on line %d\n",__FILE__,__LINE__);
        return false;
    }
    return true;
}

bool cScreen_manager::SM_set_bg(SDL_Color* fill_color,SDL_Surface* fill_image) {
    if ( fill_color != NULL) {
        Uint32 fill_int = clr_to_uint(fill_color);
        SDL_FillRect(screen, NULL, fill_int);
        return true;
    }

    if ( fill_image != NULL ) {
        apply_surface(0, 0, fill_image, screen);
        return true;
    }

    fprintf(stderr, "SM_set_bg was called with no resulting action\n");
    return false;
}

bool cScreen_manager::SM_maxFPS(int max) {
    maxFPS = max;
    return true;
}
