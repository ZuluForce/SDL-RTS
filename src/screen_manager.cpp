#include "screen_manager.h"
#include "std_gfx.h"

using namespace std;

/* Class for controlling an SDL screen */
#define CLEANUP_TIMEOUT 2000

extern bool quit_threads;
extern INIReader* pSettings;

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

    if ( pSettings ) {
        if ( pSettings->exists("Screen_Manager", "maxFPS") ) {
            this->maxFPS = pSettings->extractValue<int>("Screen_Manager", "maxFPS");
            if ( maxFPS <= 0 ) //Bad setting
				this->maxFPS = 30;
			else
				printf("Loaded setting: maxFPS = %d\n", maxFPS);
        } else {
            this->maxFPS = 30;
        }
    } else {
		this->maxFPS = 30;
    }
    fps_timer = new std_fuse();
    SM_active_thread = false;
    SM_thread_ptr = NULL;
    cleaned = false;
    pixel_depth = bpp;
    backlog = 0;

    screen_lock = SDL_CreateSemaphore(1);
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
        std_sleep(10);
    }
    SDL_FreeSurface(screen);
    free(fps_timer);
    SDL_DestroySemaphore( screen_lock );
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
        std_sleep(10);
    }

    SDL_FreeSurface( screen );
    free( fps_timer );
    cleaned = true;

    return;
}

int cScreen_manager::SM_get_w() {
    return screen->w;
}

int cScreen_manager::SM_get_h() {
    return screen->h;
}

int cScreen_manager::SM_get_depth() {
    return pixel_depth;
}

Uint32 cScreen_manager::SM_get_flags() {
    return screen->flags;
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
    backlog = 0;
    return true;
}
bool cScreen_manager::SM_set_bg(SDL_Color* fill_color,SDL_Surface* fill_image) {
    printf("SM_set_bg was called\n");
    if ( fill_color != NULL) {
        printf("Filling the screen with a color\n");
        Uint32 fill_int = clr_to_uint(fill_color);
        SDL_SemWait( screen_lock );
        SDL_FillRect(screen, NULL, fill_int);
        SDL_SemPost( screen_lock );
        return true;
    }

    if ( fill_image != NULL ) {
        SDL_SemWait( screen_lock );
        apply_surface(0, 0, fill_image, screen);
        SDL_SemPost( screen_lock );
        return true;
    }

    fprintf(stderr, "SM_set_bg was called with no resulting action\n");
    return false;
}

void cScreen_manager::SM_blit(int x, int y, SDL_Surface* src, SDL_Rect* clip) {
    SDL_SemWait( screen_lock );
    apply_surface(x, y, src, screen, clip);
    ++backlog;
    SDL_SemPost( screen_lock );
return;
}

bool cScreen_manager::SM_maxFPS(int max) {
    maxFPS = max;
    return true;
}

bool cScreen_manager::SM_backlog(int reset_if) {
    if ( backlog >= reset_if )
        return true;
    return false;
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

    int fps_interval = 1000.0 / _SM->maxFPS;
    printf("fps_interval = %d\n", fps_interval);
    printf("_SM->maxFPS = %d\n", _SM->maxFPS);

    while (quit_threads == false) {
        _SM->fps_timer->start( fps_interval );

        SDL_SemWait( _SM->screen_lock );
        _SM -> SM_update();

        SDL_SemPost( _SM->screen_lock );

		_SM->fps_timer->wait_out();
    }
    _SM->SM_active_thread = false;
    return 0;
}
