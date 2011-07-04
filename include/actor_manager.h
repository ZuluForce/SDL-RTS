#ifndef ACTOR_MANAGER_H_INCLUDED
#define ACTOR_MANAGER_H_INCLUDED

#include <vector>
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
        friend void AM_input_events(SDL_Event* event);
        friend int AM_thread_loop(void* );
        SDL_Thread* AM_start_thread();
};

void AM_input_events(SDL_Event* event);
int AM_thread_loop(void* );

#endif // ACTOR_MANAGER_H_INCLUDED
