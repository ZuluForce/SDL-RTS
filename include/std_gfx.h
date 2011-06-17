#ifndef STD_GFX_H_INCLUDED
#define STD_GFX_H_INCLUDED

#include "SDL/SDL.h"
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
SDL_Surface *load_image(const char* filename );
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
        int startTick;
        int fuseLength;

    public:
        std_fuse(int length);
        void start();
        /* Return Values:
         * true  : Fuse is still running (safe)
         * false : Fuse is burnt out (boom!)
         */
        bool check();
        void reset(bool start = false, int new_length = 0);
} std_fuse;

#endif //STD_GFX_H_INCLUDED
