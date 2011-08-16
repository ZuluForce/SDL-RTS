#include "SDL/SDL.h"
#include "std_gfx.h"

SDL_Surface* load_image(const char* filename ) {
    printf("Loading Image: %s\n",filename);
    SDL_Surface* loadedImage = NULL;

    SDL_Surface* optimizedImage = NULL;

    loadedImage = SDL_LoadBMP( filename );
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );
    } else {
        fprintf(stderr,"Failed to load image: \"%s\" : %s on line %d\n",filename,__FILE__,__LINE__);
        fprintf(stderr,"\tFail Reason: %s\n",SDL_GetError());
    }
    printf("\tImage Address: %p\n",optimizedImage);
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Temporary Offsets
    SDL_Rect offset;

    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

void std_sleep(Uint32 timeout) {
    /* Sleep for *timeout* milliseconds */
    #ifdef __MINGW32__
    Sleep(timeout);
    #elif __linux__
    SDL_Delay(timeout);
    #endif
    return;
}

Uint32 clr_to_uint(SDL_Color* color) {
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

std_timer::std_timer() {
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void std_timer::start() {
    started = true;
    paused = false;

    startTicks = SDL_GetTicks();

    return;
}

void std_timer::stop() {
    started = false;
    paused = false;
}

int std_timer::get_ticks() {
    if (started == true) {
        if ( paused == true ) {
            return pausedTicks;
        } else {
            return SDL_GetTicks() - startTicks;
        }
    }

    return 0;
}

void std_timer::pause() {
    if ( ( started == true ) && ( paused == false ) ) {
        paused = true;

        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void std_timer::unpause() {
    if ( paused == true ) {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;

        pausedTicks = 0;
    }
}

bool std_timer::is_started() {
    return started;
}

bool std_timer::is_paused() {
    return paused;
}

std_fuse::std_fuse() {
    fuseLength = 0;
    startTicks = 0;
    return;
}

void std_fuse::start(int time) {
    fuseLength = time;
    startTicks = SDL_GetTicks();
    return;
}

bool std_fuse::check() {
    if ( (SDL_GetTicks() - startTicks) >= fuseLength ) return false;
    return true;
}

void std_fuse::wait_out() {
    if ( !check() ) return;
    SDL_Delay(1);
}
