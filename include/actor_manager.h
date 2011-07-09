#ifndef ACTOR_MANAGER_H_INCLUDED
#define ACTOR_MANAGER_H_INCLUDED

#include <vector>
#include <list>
#include <cstddef>
#include "SDL/SDL.h"
#include "screen_manager.h"
#include "physics.h"
#include "priority.h"
#include "ID.h"

using namespace std;

typedef vector<SDL_Event*> event_vector;

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
        virtual void check_events(event_vector**, int* load);
        virtual sDisplay_info* get_display();
        virtual int set_priority(int);
        virtual vector<SDL_EventType>* event_binds();
};

/* Functions for mantaining order in the priority queue */

bool min_actor(void* actor1, void* actor2);

bool max_actor(void* actor1, void* actor2);

template <class T>
class proirity_stack {
    private:
        vector< list<T*> >* levels;
        vector< list<T*> >::iterator* lvl_it;
        list<T*>::iterator* walk_it;
        int (*get_priority) (T*);

    public:
        priority_stack(int init_size);
        void reg_accessor(int (*foo) (T*))
        void insert(T* obj);
        void insert(T* obj, int priority);
        T* remove(T* obj);
        T* remove(T* obj, int priority);

        T* walk();
        void reset();
}

/*------------------------------------------*/
class cActor_manager {
    private:
        SDL_Thread* AM_thread;
        cScreen_manager* SM;
        cID_dispatch* actor_id_manager;

        /* Screen Buffers */

        SDL_Surface* Draw_Buffer;

        /* Event Buffers */

        event_vector* Event_Buffer[SDL_NUMEVENTS];
        int event_buf_load[SDL_NUMEVENTS];

        /* Structures for the storage and sorting of
         * Actors according to their priority and
         * type */

         p_queue* actor_objs;

    public:
        cActor_manager(cScreen_manager*);
        void AM_register(cActor* obj);
        void AM_blit_buffer(int x, int y, SDL_Surface* src, SDL_Rect* clip = NULL);
        void AM_blit_buffer(sDisplay_info*);
        void AM_flip_buffer();
        void AM_update();
        friend void AM_input_events(SDL_Event* event);
};

void AM_input_events(SDL_Event* event);

#endif // ACTOR_MANAGER_H_INCLUDED
