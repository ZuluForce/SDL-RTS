#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

struct phys_cont; //so that the import of actor_manager.h will work
struct sDisplay_info;

#include <cmath>
#include <vector>
#include <list>
#include "actor_manager.h"
#include "ID.h"
#include "SDL/SDL.h"

extern cActor_manager* pAM;

typedef pair<int,int> coordinates;

union params {
    //Type 0
    coordinates* w_h;
    //Type 1
    int radius;
};

typedef struct phys_cont {
    public:
        /* 0 = Rectangle
         * 1 = Circle
         */
        int contType;
        int actorType; //Filed in by actor manager or creator of actor

        /* 0 = Static, nothing can move it
         * 1 = Elastic
         * 2 = Inelastic
         */
        int collType;
        int PM_ID; //For use by the physics manager

        sDisplay_info* obj_info; //User must fill before register

        int x; //User must fill before register if obj_info isn't
        int y;

        int x_vel, y_vel, tx_vel, ty_vel;
        int x_accel, y_accel, tx_accel, ty_accel;
        int mass;

        params param;
        int level;

        void init(int contType, int level = 0);

    private:
        //[min_x,min_y,max_x,max_y]
        int* move_bounds[4];
        //Used to determine what direction an object is being moved
        //so collision responses can be handled correctly
        coordinates* move_direction;

        vector< coordinates > grid_locations;

        coordinates* coor_buffer;

        short gravity;

} phys_cont;

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
        list<phys_cont*>*** collision_zone_grid;
        list<phys_cont*>*** collision_obj_grid;
        short** obj_grid_load;

        cID_dispatch obj_id_manage;

        void PM_init_grid_loc_0(phys_cont*);

        //Called with the type of the first object
        /* Knowing the type of the first object, they call
           the given check function for all those it could
           possibly collide with */
        coordinates* PM_check_rect_(phys_cont*);
        coordinates* PM_check_circle_(phys_cont*);

        //These modify the coor_buffer inside the phys_container
        void PM_check_rect_rect(phys_cont*,phys_cont*);
        void PM_check_rect_circle(phys_cont*,phys_cont*);
        void PM_check_circle_circle(phys_cont*,phys_cont*);

        coordinates* PM_resolve_collision(coordinates*,coordinates*);

    public:
        cPhysic_manager(int grid_width, int grid_height);
        /* Sets up a collision zone so any colision objects cannot pass through it */
        /* If nothing is passed for level it will collide with objects across all levels */
        void PM_set_collide_zone(int x, int y, params parameters, int type = 0, int level = -1);
        void PM_register_collision_obj(phys_cont*);
        coordinates* PM_check_collision(phys_cont*,bool);
        void PM_move(phys_cont*,int x, int y);
        void PM_set_pos(phys_cont*, int x, int y);
        void PM_set_vel(phys_cont*, int velocity);
        void PM_set_accel(phys_cont*, int accel);
};

#endif // PHYSICS_H_INCLUDED
