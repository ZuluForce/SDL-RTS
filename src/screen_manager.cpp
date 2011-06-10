#include "screen_manager.h"

using namespace std;

/* Class for controlling an SDL screen */

typedef class {
    private:
        SDL_Surface* screen;
        bool visible;

    public:
        bool cScreen_manager(int width, int height, int bpp, Uint32 flags);

        /* May need to be changed to const char* */
        bool SM_set_caption(string* cap, string* icon);
        bool SM_show();
        bool SM_set_bg();
} cScreen_manager;

bool cScreen_manager::cScreen_manager(int width, int height, int bpp, Uint32 flags, bool show) {
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1) {
        fprintf(stderr, "SDL failed to initialize : %s on line %d\n", __FILE__, __LINE__);
        return false;
    }

    screen = SDL_SetVideoMode(width, height, bpp, flags);

    if ( screen == NULL) {
        fprintf(stderr, "Failed to initialize the screen : %s on line %d\n",__FILE__,__LINE__);
        return false;
    }

    if ( show ) SDL_Flip(screen);
}

bool cScreen_manager::SM_set_caption(string* cap, string* icon) {
    SDL_WM_SetCaption(cap, icon);
    return true;
}

bool cScreen_manager::SM_set_bg(SDL_Color* fill_color == NULL,SDL_Surface* fill_image == NULL) {
    if ( fill_color != NULL) {
        SDL_FillRect(screen, NULL, fill_color);
        return true;
    }

    if ( fill_image != NULL ) {
        apply_image(0, 0, fill_image, dst);
        return true;
    }

    fprintf(stderr, "SM_set_bg was called with no resulting action\n");
    return false;
}
