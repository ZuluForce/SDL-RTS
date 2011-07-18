#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include "SDL/SDL.h"

typedef struct {

    int x;
    int y;

    int width;
    int height;

    int level;
} phys_rect;

typedef class Particle {
    private:
        int x,y;
        int frame;

        SDL_Surface *type;

    public:
        Particle(int x, int y);
        void show();
        bool is_dead();

} cParticle;

class cPhysic_manager{
    private:
        int grid_w, grid_h;
        phys_rect*** collision_zone_grid;

    public:
        cPhysic_manager(int grid_width, int grid_height, int levels);
        /* Sets up a collision zone so any colision objects cannot pass through it */
        /* If nothing is passed for level it will collide with objects across all levels */
        void PM_set_collide_zone(int x, int y, int width, int height, int level = -1);
        phys_rect** PM_merge_rects(phys_rect*,phys_rect*);
};

#endif // PHYSICS_H_INCLUDED
