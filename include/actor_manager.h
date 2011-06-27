#ifndef ACTOR_MANAGER_H_INCLUDED
#define ACTOR_MANAGER_H_INCLUDED

#include <cstddef>
#include "physics.h"

struct sDisplay_info {
    SDL_Surface* surf;
    int x,y;
    SDL_Rect* clip;
}

class cActor_manager {
    private:
        void AM_thread_loop(void* );
        SDL_Thread* AM_thread;
        cID_dispatch* actor_id_manager;

        int static_count;
        int static_max;
        cActor_static** static_objs;
        int dynamic_count;
        int dynamic_max;
        cActor_dynamic** dynamic_objs;
        int active_count;
        int active_max;
        cActor_active** active_objs;
    public:
        cActor_manager();
        void AM_register(cActor_static* obj);
        void AM_register(cActor_dynamic* obj);
        void AM_register(cActor_active* obj);
        SDL_Thread* AM_start_thread();
} cActor_manager;

class cActor {
    private:
        int ID,typeID;
        int priority;
        phys_rect* rect;
        SDL_Surface* image;
        bool update;

    public:
        virtual bool check();
        virtual sDisplay_info* get_display();

}

class cActor::cActor_static {
    public:
        virtual int set_priority(int);
}

class cActor::cActor_dynamic {
    ;
}

class cActor::cActor_active {
    ;
}

#endif // ACTOR_MANAGER_H_INCLUDED
