#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#inlcude <vector>
#include <list>
#include "SDL/SDL.h"

typedef struct {

    int x;
    int y;

    int width;
    int height;

    int level;

    vector< pair<int,int> > grid_locations;
    int rectID;
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
        list<phys_rect*>** collision_zone_grid;
        list<phys_rect*>** collision_obj_grid;

        cID_dispatch obj_id_manage;

    public:
        cPhysic_manager(int grid_width, int grid_height, int levels);
        /* Sets up a collision zone so any colision objects cannot pass through it */
        /* If nothing is passed for level it will collide with objects across all levels */
        void PM_set_collide_zone(int x, int y, int width, int height, int level = -1);
        void PM_register_collision_obj(phys_rect*);
        phys_rect** PM_merge_rects(phys_rect*,phys_rect*);
};

#endif // PHYSICS_H_INCLUDED
