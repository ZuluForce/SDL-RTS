#include "actor_manager.h"

#define EVENT_BUFFER_INIT 50

extern bool quit_threads;

/* Functions for mantaining order in the priority queue */

bool min_actor(void* actor1, void* actor2) {
    if ( ((cActor* ) actor1)->priority > ((cActor* ) actor2)->priority ) {
        return false;
    }
    return true;
}


bool max_actor(void* actor1, void* actor2) {
    if ( ((cActor* ) actor1)->priority < ((cActor* ) actor2)->priority ) {
        return false;
    }
    return true;
}

/*-----------------------------------------------*/

cActor_manager::cActor_manager() {
    AM_thread = NULL;
    cID_dispatch* actor_id_manager = new cID_dispatch();

    Draw_Buffer = NULL;

    //Redo using the C++ vector class
    Event_Buffer = (SDL_Event** ) malloc( SDL_NUMEVENTS * sizeof( SDL_Event* ) * EVENT_BUFFER_INIT );

    actor_objs = new_pqueue(100, true, min_actor);

    return;
}

void cActor_manager::AM_register(cActor* obj) {
    obj->ID = actor_id_manager->ID_getid();

    pq_insert(actor_objs, obj);

    return;
}

SDL_Thread* cActor_manager::AM_start_thread() {
    SDL_Thread* new_thread = SDL_CreateThread( AM_thread_loop, this);
    return new_thread;
}

int AM_thread_loop(void* AM) {
    //cActor_manager* _AM = (cActor_manager* ) AM;

    while ( quit_threads == false ) {
        SDL_Delay(10);
    }
    return 0;
}
