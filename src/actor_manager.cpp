#include "actor_manager.h"

#define EVENT_BUFFER_INIT 20

extern bool quit_threads;

cActor_manager* event_listener = NULL;

/* Functions for mantaining order in the priority queue */

bool min_actor(void* actor1, void* actor2) {
    if ( ((cActor* ) actor1)->priority > ((cActor* ) actor2)->priority ) {
        return true;
    }
    return false;
}


bool max_actor(void* actor1, void* actor2) {
    if ( ((cActor* ) actor1)->priority < ((cActor* ) actor2)->priority ) {
        return true;
    }
    return false;
}

/*-----------------------------------------------*/

cActor_manager::cActor_manager(cScreen_manager* _SM) {
    AM_thread = NULL;
    SM = _SM;
    cID_dispatch* actor_id_manager = new cID_dispatch();

    Draw_Buffer = NULL;

    int i;
    for (i = 0; i < SDL_NUMEVENTS; ++i) {
        Event_Buffer[i] = new event_vector (EVENT_BUFFER_INIT);
        event_buf_load[i] = -1;
    }

    actor_objs = new_pqueue(100, true, min_actor);
    event_listener = this;

    return;
}

void cActor_manager::AM_register(cActor* obj) {
    obj->ID = actor_id_manager->ID_getid();

    pq_insert(actor_objs, obj);

    vector<SDL_EventType>* events = obj->event_binds();
    Uint16 i;
    for (i = 0; i < events->size(); ++i) {
        if ( event_buf_load[events->at(i)] == -1) {
            /* Signals that we want to collect events of this type */
            event_buf_load[events->at(i)] = 0;
        }
    }

    return;
}

void cActor_manager::AM_blit_buffer(int x, int y, SDL_Surface* src, SDL_Rect* clip) {
    apply_surface(x,y,src,Draw_Buffer,clip);
    return;
}

void cActor_manager::AM_blit_buffer(sDisplay_info* sdi) {
    apply_surface(sdi->x, sdi->y, sdi->surf, Draw_Buffer, sdi->clip);
    return;
}

void cActor_manager::AM_flip_buffer() {
    SM->SM_blit(0,0,Draw_Buffer,NULL);
    return;
}

SDL_Thread* cActor_manager::AM_start_thread() {
    SDL_Thread* new_thread = SDL_CreateThread( AM_thread_loop, this);
    return new_thread;
}

void AM_input_events(SDL_Event* event) {
    //printf("Actor Manager recieved event of type: %d\n",event->type);
    cActor_manager* _AM = event_listener;
    int type = event->type;

    if ( _AM->event_buf_load[type] == -1) return;

    _AM->Event_Buffer[type]->at( _AM->event_buf_load[type]++ ) = event;
    return;
}

int AM_thread_loop(void* AM) {
    cActor_manager* _AM = (cActor_manager* ) AM;
    cActor* actor_update;
    sDisplay_info* actor_info;

    while ( quit_threads == false ) {
        /* Lock the Event Buffer */
        /* Send out Event Updates */
        while ( (actor_update == (cActor*)pq_extract(_AM->actor_objs)) != NULL ) {
            actor_update->check_events(_AM->Event_Buffer, _AM->event_buf_load);
        }
        pq_insert(_AM->actor_objs, actor_update);
        /*------------------------*/
        /*------------------------*/

        /* Check which objects need re-blitting */
        while ( (actor_update == (cActor* ) pq_extract(_AM->actor_objs)) != NULL ) {
            if ( actor_update->check() ) {
                actor_info = actor_update->get_display();
                _AM->AM_blit_buffer(actor_info);
            }
        }
        /*--------------------------------------*/
        //SDL_Delay(10);
        std_sleep(10);
    }
    return 0;
}
