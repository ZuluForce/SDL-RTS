#include "screen_manager.h"
#include "std_gfx.h"

using namespace std;

/* Class for controlling an SDL screen */
#define CLEANUP_TIMEOUT 2000

extern bool quit_threads;

/* --------------------- Private SM Functions ------------------------- */
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

/* ----------------------- End Private SM Functions --------------------- */

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
    SM_active_thread = false;
    SM_thread_ptr = NULL;
    cleaned = false;
    return;
}

cScreen_manager::~cScreen_manager() {
    if ( cleaned ) return;
    std_fuse cleanupFuse = std_fuse();
    cleanupFuse.start( CLEANUP_TIMEOUT );

    while ( SM_active_thread ) {
        if ( !cleanupFuse.check() ) {
            fprintf(stderr, "Timeout while waiting to cleanup the Screen Manager.\n");
            SDL_KillThread( SM_thread_ptr );
            return;
        }
        SDL_Delay(10);
    }
    SDL_FreeSurface(screen);
    free(fps_timer);
    return;
}

/* This is incase you want to call SDL_Quit before the destructor is called */
void cScreen_manager::cleanup(int timeout) {
    std_fuse cleanupFuse = std_fuse();
    cleanupFuse.start( timeout );

    while ( SM_active_thread ) {
        if ( !cleanupFuse.check() ) {
            fprintf(stderr, "Timeout while waiting to cleanup the Screen Manager.\n");
            SDL_KillThread( SM_thread_ptr );
            return;
        }
        SDL_Delay(10);
    }

    SDL_FreeSurface( screen );
    free( fps_timer );
    cleaned = true;

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

bool cScreen_manager::SM_update() {
    if ( SDL_Flip(screen) == -1 ) {
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

/* To be used as a threaded function
 * to automatically update the screen
 * at the frame rate specified in SM
 */

SDL_Thread* SM_start(cScreen_manager* SM) {
    SM->SM_thread_ptr = SDL_CreateThread(start_SM_thread,SM);

    if ( SM->SM_thread_ptr == NULL ) {
        fprintf(stderr, "Failed to start SM thread: %s on line %d\n",__FILE__,__LINE__);
        return NULL;
    }
    return SM->SM_thread_ptr;
}

bool SM_wait_to_finish(cScreen_manager* SM, int timeout) {
    std_fuse wait_timeout = std_fuse();
    wait_timeout.start( timeout );
    while ( SM->SM_active_thread ) {
        if ( !wait_timeout.check() ) {
            fprintf(stderr, "Timeout while waiting to cleanup the Screen Manager.\n");
            return false;
        }
        SDL_Delay(10);
    }
    return true;
}

int start_SM_thread(void* SM) {
    cScreen_manager* _SM = (cScreen_manager* ) SM;
    _SM->SM_active_thread = true;

    SDL_Color black = {255,255,255};
    SDL_Color white = {0,0,0};

    bool screen_color = true;

    while (quit_threads == false) {

        /* Makes the screen flash black/white */
        if ( screen_color ) {
            screen_color = false;
            _SM->SM_set_bg(&white);
        } else {
            screen_color = true;
            _SM->SM_set_bg(&black);
        }

        /*------------------------------------*/

        _SM -> SM_update();
        SDL_Delay(6);
    }
    _SM->SM_active_thread = false;
    return 0;
}
