#ifndef STD_GFX_H_INCLUDED
#define STD_GFX_H_INCLUDED

#include "SDL/SDL.h"
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
SDL_Surface *load_image(const char* filename );

typedef class std_timer {
    private:
        int startTicks;
        int pausedTicks;

        bool paused;
        bool started;

    public:
        std_timer();

        void start();
        void stop();
        void pause();
        void unpause();

        int get_ticks();

        bool is_started();
        bool is_paused();
} std_timer;

#endif //STD_GFX_H_INCLUDED
