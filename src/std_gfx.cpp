#include "SDL/SDL.h"

SDL_Surface *load_image(const char* filename ) {
    SDL_Surface* loadedImage = NULL;

    SDL_Surface* optimizedImage = NULL;

    loadedImage = SDL_LoadBMP( filename );

    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );

        SDL_FreeSurface( loadedImage );
    }
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Temporary Offsets
    SDL_Rect offset;

    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

Timer::Timer() {
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start() {
    started = true;
    paused = false;

    startTicks = SDL_GetTicks();

    return;
}

void Timer::stop() {
    started = false;
    paused = false;
}

int Timer::get_ticks() {
    if (started == true) {
        if ( paused == true ) {
            return pausedTicks;
        } else {
            return SDL_GetTicks() - startTicks;
        }
    }

    return 0;
}

void Timer::pause() {
    if ( ( started == true ) && ( paused == false ) ) {
        paused = true;

        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause() {
    if ( paused == true ) {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;

        pausedTicks = 0;
    }
}

bool Timer::is_started() {
    return started;
}

bool Timer::is_paused() {
    return paused;
}
