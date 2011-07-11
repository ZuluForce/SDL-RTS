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
    SM = _SM;

    Draw_Buffer = SDL_CreateRGBSurface(_SM->SM_get_flags(),_SM->SM_get_w(), _SM->SM_get_h(), _SM->SM_get_depth(),
                                       0,0,0,0);

    if (Draw_Buffer == NULL) {
        fprintf(stderr,"Failed to create the Draw Buffer: %s on line %d\n",__FILE__,__LINE__);
        fprintf(stderr,"\tError: %s\n",SDL_GetError());
    }

    int i;
    for (i = 0; i < SDL_NUMEVENTS; ++i) {
        Event_Buffer[i] = new event_vector (EVENT_BUFFER_INIT);
        event_buf_load[i] = -1;
    }

    actor_objs = priority_stack<cActor*>(10);
    actor_objs.reg_accessor(Actor_Priority);
    actor_objs.reg_IDaccessor(Actor_PriorityID);
    actor_objs.reg_IDmodifier(Actor_modID);

    event_listener = this;

    return;
}

void cActor_manager::AM_register(cActor* obj) {

    actor_objs.insert(obj);

    vector<Uint8>* events = obj->event_binds();
    Uint16 i;
    for (i = 0; i < events->size(); ++i) {
        if ( event_buf_load[events->at(i)] == -1) {
            /* Signals that we want to collect events of this type */
            printf("Actor Manager is now collecting Events of Type (%d)\n",events->at(i));
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

void cActor_manager::AM_clear_load_buf() {
    for (int i = 0; i < SDL_NUMEVENTS; ++i) {
        if (event_buf_load[i] != -1) {
            event_buf_load[i] = 0;
        }
    }
    return;
}
void AM_input_events(SDL_Event* event) {
    //printf("Actor Manager recieved event of type: %d\n",event->type);
    cActor_manager* _AM = event_listener;
    int type = event->type;

    if ( _AM->event_buf_load[type] == -1) return;

    _AM->Event_Buffer[type]->at( _AM->event_buf_load[type]++ ) = *event;
    return;
}

void cActor_manager::AM_update() {
    cActor* actor_update = NULL;
    sDisplay_info* actor_info;
    Uint8* key_states = SDL_GetKeyState(NULL);

    /* Send out Event Updates */
    actor_update = actor_objs.walk();
    //while ( (actor_update = (cActor*)pq_top( actor_objs )) != NULL ) {
        actor_update->check_events(Event_Buffer, event_buf_load, key_states);
    //}

    /*------------------------*/
    /* Check which objects need re-blitting */
    //while ( (actor_update = (cActor* ) pq_top( actor_objs )) != NULL ) {
        if ( actor_update->check() ) {
            actor_info = actor_update->get_display();
            AM_blit_buffer(actor_info);
        }
    //}

    AM_flip_buffer();
    /*--------------------------------------*/
    AM_clear_load_buf();
    return;
}

int Actor_Priority(cActor* actor) {
    return actor->priority;
}

int Actor_PriorityID(cActor* actor) {
    return actor->priorityID;
}

void Actor_modID(cActor* actor, int id) {
    actor->priorityID = id;
    return;
}
