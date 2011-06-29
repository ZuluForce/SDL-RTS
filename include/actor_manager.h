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

class cActor {
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

bool min_actor(void* actor1, void* actor2);

bool max_actor(void* actor1, void* actor2);

/*------------------------------------------*/

class cActor_manager {
    private:
        SDL_Thread* AM_thread;
        cID_dispatch* actor_id_manager;

        /* Screen Buffers */

        SDL_Surface* Draw_Buffer;

        /* Event Buffers */

        SDL_Event** Event_Buffer;

        /* Structures for the storage and sorting of
         * Actors according to their priority and
         * type */

         p_queue* actor_objs;

    public:
        cActor_manager();
        void AM_register(cActor* obj);
        SDL_Thread* AM_start_thread();
};

int AM_thread_loop(void* );

#endif // ACTOR_MANAGER_H_INCLUDED
