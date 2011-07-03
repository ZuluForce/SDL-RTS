#ifndef STD_GFX_H_INCLUDED
#define STD_GFX_H_INCLUDED

#include "SDL/SDL.h"
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
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

#endif //STD_GFX_H_INCLUDED
