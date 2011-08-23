#ifndef STD_GFX_H_INCLUDED
#define STD_GFX_H_INCLUDED

#include <ctime>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#ifdef __MINGW32__

#include  <windows.h>

#endif

#ifdef __GNUC__

#include <stdlib.h>

#endif

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
SDL_Surface *load_image(const char* filename );
void std_sleep(Uint32 timeout);
Uint32 clr_to_uint(SDL_Color* color);

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

typedef class std_fuse {
    private:
        int startTicks;
        int fuseLength;

    public:
        std_fuse();
        void start(int time);
        /* Return Values:
         * true  : Fuse is still running (safe)
         * false : Fuse is burnt out (boom!)
         */
        bool check();
        void wait_out();
} std_fuse;

typedef class std_scale {
    private:
        clock_t start_tick;
    public:
        void set_scale(int msec);
        void start();
        float measure();
} std_scale;

#endif //STD_GFX_H_INCLUDED
