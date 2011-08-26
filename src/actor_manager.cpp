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

bool cActor::check() {
    return update;
}

void cActor::check_events(event_vector**, int* load, Uint8* key_states) {
    if ( !update ) return;
    return;
}

sDisplay_info* cActor::get_display() {
    if ( update ) {
        return &curr_info;
    }
    return NULL;
}

int cActor::set_priority(int i) {
    priority = i;
    return i;
}

vector<Uint8>* cActor::event_binds() {
    return &_event_binds;
}

/*-----------------------------------------------*/

cActor_manager::cActor_manager(cScreen_manager* _SM) {
    SM = _SM;

    Draw_Buffer = SDL_CreateRGBSurface(_SM->SM_get_flags(),_SM->SM_get_w(), _SM->SM_get_h(), _SM->SM_get_depth(),
                                       0,0,0,0);

    printf("The draw buffer address is: %p\n", Draw_Buffer);

    if (Draw_Buffer == NULL) {
        fprintf(stderr,"Failed to create the Draw Buffer: %s on line %d\n",__FILE__,__LINE__);
        fprintf(stderr,"\tError: %s\n",SDL_GetError());
    }
    back_type = 0;

    int i;
    for (i = 0; i < SDL_NUMEVENTS; ++i) {
        Event_Buffer[i] = new event_vector (EVENT_BUFFER_INIT);
        event_buf_load[i] = -1;
    }
    /* CHANGE THIS FOR HOW MANY LEVELS YOU NEED */
    actor_objs = priority_stack<cActor*>(10);
    /*------------------------------------------*/

    actor_objs.reg_accessor(Actor_Priority);
    actor_objs.reg_IDaccessor(Actor_PriorityID);
    actor_objs.reg_IDmodifier(Actor_modID);

    event_listener = this;

    return;
}

void cActor_manager::AM_register(cActor* obj) {

    actor_objs.insert(obj);

    vector<Uint8>* events = obj->event_binds();
    if ( !events ) return;
    Uint16 i;
    for (i = 0; i < events->size(); ++i) {
        if ( event_buf_load[events->at(i)] == -1) {
            /* Signals that we want to collect events of this type */
            printf("Actor Manager is now collecting Events of Type (%d)\n",events->at(i));
            event_buf_load[events->at(i)] = 0;
        }
    }
    actor_objs.reset_walk();
    return;
}

void cActor_manager::AM_set_bg(SDL_Color* fill_color) {
    if ( fill_color != NULL ) {
        back_type = 1;
        Back_Color = clr_to_uint(fill_color);
        return;
    }
    back_type = 0;
    fprintf(stderr, "AM_set_bg (color) was called with no result\n");
}

void cActor_manager::AM_set_bg(SDL_Surface* fill_surf) {
    if (fill_surf != NULL) {
        back_type = 2;
        Background = fill_surf;
        return;
    }
    back_type = 0;
    fprintf(stderr, "AM_set_bg (Surface) was called with no result\n");
}

void cActor_manager::AM_blit_buffer(int x, int y, SDL_Surface* src, SDL_Rect* clip) {
    apply_surface(x,y,src,Draw_Buffer,clip);
    return;
}

void cActor_manager::AM_blit_buffer(sDisplay_info* sdi) {
    if ( sdi == NULL) return;
    apply_surface(sdi->x, sdi->y, sdi->surf, Draw_Buffer, sdi->clip);
    return;
}

void cActor_manager::AM_blit_buffer(Uint32 fill) {
    SDL_FillRect(Draw_Buffer,NULL, fill);
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
    while ( (actor_update = actor_objs.walk()) != NULL ) {
        actor_update->check_events(Event_Buffer, event_buf_load, key_states);
    }
    /*------------------------*/
    actor_objs.reset_walk();

    switch ( back_type ) {
        case 0:
            break;
        case 1:
            AM_blit_buffer(Back_Color);
            break;
        case 2:
            AM_blit_buffer(0,0,Background,NULL);
            break;
        default:
            break;
    }

    /* Check which objects need re-blitting */
    while ( ( actor_update = actor_objs.walk() ) != NULL ) {
        //if ( actor_update->check() ) {
            actor_info = actor_update->get_display();
            AM_blit_buffer(actor_info);
        //}
    }

    AM_flip_buffer();
    /*--------------------------------------*/
    AM_clear_load_buf();
    actor_objs.reset_walk();
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
