#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include "SDL/SDL.h"

typedef struct {

    int x;
    int y;

    int width;
    int height;
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

typedef class cPhysic_manager{
    public:
        cPhysic_manager();
} cPhysic_manager;

#endif // PHYSICS_H_INCLUDED
