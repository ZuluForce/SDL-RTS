#include "actor_manager.h"

#define INIT_ACTOR_ARRAY_SIZE 30

extern bool quit_threads;

/* Functions for mantaining order in the priority queue */

bool min_static(void* actor1, void* actor2) {
    if ( ((cActor_static* ) actor1)->priority > ((cActor_static* ) actor2)->priority ) {
        return false;
    }
    return true;
}


bool max_static(void* actor1, void* actor2) {
    if ( ((cActor_static* ) actor1)->priority < ((cActor_static* ) actor2)->priority ) {
        return false;
    }
    return true;
}

bool min_dynamic(void* actor1, void* actor2) {
    if ( ((cActor_dynamic* ) actor1)->priority > ((cActor_dynamic* ) actor2)->priority ) {
        return false;
    }
    return true;
}


bool max_dynamic(void* actor1, void* actor2) {
    if ( ((cActor_dynamic* ) actor1)->priority < ((cActor_dynamic* ) actor2)->priority ) {
        return false;
    }
    return true;
}

bool min_active(void* actor1, void* actor2) {
    if ( ((cActor_active* ) actor1)->priority > ((cActor_active* ) actor2)->priority ) {
        return false;
    }
    return true;
}


bool max_active(void* actor1, void* actor2) {
    if ( ((cActor_active* ) actor1)->priority < ((cActor_active* ) actor2)->priority ) {
        return false;
    }
    return true;
}

/*-----------------------------------------------*/

cActor_manager::cActor_manager() {
    AM_thread = NULL;
    cID_dispatch* actor_id_manager = new cID_dispatch();

    static_objs = new_pqueue(40, true, min_static);
    dynamic_ojbs = new_pqueue(40, true, min_dynamic);
    active_objs = new_pqueue(40, true, min_active);

    /*
    static_objs = malloc(INIT_ACTOR_ARRAY_SIZE * sizeof(cActor_static*));
    dynamic_objs = malloc(INIT_ACTOR_ARRAY_SIZE * sizeof(cActor_dynamic*));
    active_objs = malloc(INIT_ACTOR_ARRAY_SIZE * sizeof(cActor_active*));

    static_count = dynamic_count = active_count = 0;
    static_max = dynamic_max = active_max = INIT_ACTOR_ARRAY_SIZE;
    */

    return;
}

void cActor_manager::AM_register(cActor_static* obj) {
    obj->ID = actor_id_manager->ID_getid();

    //static_objs[static_count++] = obj;

    pq_insert(static_objs, obj);

    return;
}

void cActor_manager::AM_register(cActor_dynamic* obj) {
    obj->ID = actor_id_manager->ID_getid();

    //dynamic_objs[dynamic_count++] = obj;

    pq_insert(dynamic_ojbs, obj);
    return;
}

void cActor_manager::AM_register(cActor_active* obj) {
    obj->ID = actor_id_manager->ID_getid();

    //active_objs[active_count++] = obj;

    pq_insert(active_objs, obj);
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
