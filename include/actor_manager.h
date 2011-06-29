#ifndef ACTOR_MANAGER_H_INCLUDED
#define ACTOR_MANAGER_H_INCLUDED

#include <cstddef>
#include "SDL/SDL.h"
#include "physics.h"
#include "priority.h"
#include "ID.h"

struct sDisplay_info {
    SDL_Surface* surf;
    int x,y;
    SDL_Rect* clip;
};

class cActor_static {
    protected:
        phys_rect* rect;
        SDL_Surface* image;
        bool update;

    public:
        int ID,typeID;
        int priority;

        virtual bool check();
        virtual sDisplay_info* get_display();
        virtual int set_priority(int);
};

class cActor_dynamic {
    protected:
        phys_rect* rect;
        SDL_Surface* image;
        bool update;

    public:
        int ID,typeID;
        int priority;

        virtual bool check();
        virtual sDisplay_info* get_display();
        virtual int set_priority(int);
};

class cActor_active {
    protected:
        phys_rect* rect;
        SDL_Surface* image;
        bool update;

    public:
        int ID,typeID;
        int priority;

        virtual bool check();
        virtual sDisplay_info* get_display();
        virtual int set_priority(int);
};

/* Functions for mantaining order in the priority queue */

bool min_static(void* actor1, void* actor2);

bool max_static(void* actor1, void* actor2);

bool min_dynamic(void* actor1, void* actor2);

bool max_dynamic(void* actor1, void* actor2);

bool min_active(void* actor1, void* actor2);

bool max_active(void* actor1, void* actor2);

/*------------------------------------------*/

class cActor_manager {
    private:
        SDL_Thread* AM_thread;
        cID_dispatch* actor_id_manager;

        /* Screen Buffer */

        SDL_Surface* Static_Surf;
        SDL_Surface* Draw_Buffer;

        /* Structures for the storage and sorting of
         * Actors according to their priority and
         * type */

         p_queue* static_objs;
         p_queue* dynamic_ojbs;
         p_queue* active_objs;

        /*
        int static_count;
        int static_max;
        cActor_static** static_objs;
        int dynamic_count;
        int dynamic_max;
        cActor_dynamic** dynamic_objs;
        int active_count;
        int active_max;
        cActor_active** active_objs;
        */

    public:
        cActor_manager();
        void AM_register(cActor_static* obj);
        void AM_register(cActor_dynamic* obj);
        void AM_register(cActor_active* obj);
        SDL_Thread* AM_start_thread();
};

int AM_thread_loop(void* );

#endif // ACTOR_MANAGER_H_INCLUDED
