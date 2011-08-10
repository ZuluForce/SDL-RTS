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

        int x,tx;
        int y,ty;

        bool new_x,new_y;

        int x_vel, y_vel, tx_vel, ty_vel;
        int x_accel, y_accel, tx_accel, ty_accel;
        int mass;

        params param;
        int level;

        void init(int contType = 0, int level = 0, int actorType = -1);

        //[min_x,min_y,max_x,max_y]
        int* move_bounds[4];
        //Used to determine what direction an object is being moved
        //so collision responses can be handled correctly
        coordinates* move_direction;
        int x_span,y_span;

        vector< coordinates >* grid_locations;

        coordinates* coor_buffer;

        short gravity;

} phys_cont;

struct collision_zone {
    int x,y;
    int contType;
    int PM_ID;
    params param;
};

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
        int screen_w, screen_h;

        list<collision_zone*>*** collision_zone_grid;
        list<phys_cont*>*** collision_obj_grid;

        cID_dispatch obj_id_manage;

        void PM_init_grid(int,int);
        void PM_init_grid_loc_0(phys_cont*);
        void PM_reset_grid_loc(phys_cont*);

        void PM_remove_from_grid(phys_cont*, int, int);
        void PM_add_to_grid(phys_cont*,int,int);

        //Called with the type of the first object
        /* Knowing the type of the first object, they call
           the given check function for all those it could
           possibly collide with */
        void PM_check_rect_(phys_cont*);
        //coordinates* PM_check_circle_(phys_cont*);

        //These modify the coor_buffer inside the phys_container
        void PM_check_rect_rect(phys_cont*,phys_cont*);
        void PM_check_rect_zone(phys_cont*,collision_zone*);
        //void PM_check_rect_circle(phys_cont*,phys_cont*);
        //void PM_check_circle_circle(phys_cont*,phys_cont*);

        coordinates* PM_resolve_collision(coordinates*,coordinates*);

    public:
        cPhysic_manager(int grid_width, int grid_height,
                        int screen_width = -1, int screen_height = -1);
        /* Sets up a collision zone so any colision objects cannot pass through it */
        /* If nothing is passed for level it will collide with objects across all levels */
        void PM_set_collide_zone(int x, int y, params* _param, int _type = 0);
        void PM_register_collision_obj(phys_cont*);
        void PM_check_collision(phys_cont*,bool);
        /* Use this for objects with discrete movements (non-continual).
           It will check for collisions but reactions will be different than
           usual. */
        void PM_move(phys_cont*,int x, int y);
        /* Use this to check for collisions and update according to
           current velocity and acceleration */
        void PM_update(phys_cont*);
        void PM_set_pos(phys_cont*, int x, int y);
        void PM_set_velocity(phys_cont*, int x_vel, int y_vel);
        void PM_set_x_velocity(phys_cont* cont, int x);
        void PM_set_y_velocity(phys_cont* cont, int y);
        void PM_set_accel(phys_cont*, int accel);

        int PM_correct_x(int x);
        int PM_correct_y(int y);

        void PM_print_grid();
};

#endif // PHYSICS_H_INCLUDED
