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
        int priority, priorityID;

        virtual bool check();
        virtual void check_events(event_vector**, int* load);
        virtual sDisplay_info* get_display();
        virtual int set_priority(int);
        virtual vector<SDL_EventType>* event_binds();
};

/* Functions for mantaining order in the priority queue */

bool min_actor(void* actor1, void* actor2);

bool max_actor(void* actor1, void* actor2);

template <typename T>
class priority_stack {
    private:
        list<T>** levels;
        typename list<T>::iterator lvl_it;
        int (*get_priority) (T);
        int (*get_priorityID) (T);
        void (*mod_priorityID) (T,int);

        cID_dispatch priority_id_manager;

    public:
        priority_stack (int init_size = 10);
        void reg_accessor(int (*foo) (T));
        void reg_IDaccessor(int (*foo) (T));
        void reg_IDmodifier(void (*foo) (T,int));
        void insert(T obj);
        T remove(T obj);

        T walk();
        void reset();
};

/*------------------------------------------*/
class cActor_manager {
    private:
        SDL_Thread* AM_thread;
        cScreen_manager* SM;

        /* Screen Buffers */

        SDL_Surface* Draw_Buffer;

        /* Event Buffers */

        event_vector* Event_Buffer[SDL_NUMEVENTS];
        int event_buf_load[SDL_NUMEVENTS];

        /* Structures for the storage and sorting of
         * Actors according to their priority and
         * type */

         priority_stack<cActor*> actor_objs;

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
int Actor_Priority(cActor*);
int Actor_PriorityID(cActor*);
void Actor_modID(cActor*,int);

/* Definitions of template functions */

template <class T>
priority_stack<T>::priority_stack(int init_size) {
    levels = (list<T>** )malloc(init_size * sizeof( list<T>* ));
    for (int i = 0; i < init_size; ++i) {
        levels[i] = new list<T>;
    }

    priority_id_manager = cID_dispatch();
    lvl_it = levels[0]->begin();
    return;
}

template <class T>
void priority_stack<T>::reg_accessor(int (*foo) (T)) {
    get_priority = foo;
    return;
}

template <class T>
void priority_stack<T>::reg_IDaccessor(int (*foo) (T)) {
    get_priorityID = foo;
    return;
}

template <class T>
void priority_stack<T>::reg_IDmodifier(void (*foo) (T,int)) {
    mod_priorityID = foo;
    return;
}

template <class T>
void priority_stack<T>::insert(T obj) {
    int priority = get_priority(obj);
    mod_priorityID(obj,priority_id_manager.ID_getid());
    levels[priority]->push_back(obj);
    printf("Priority Stack inserted object with address %p at lvl (%d)\n",obj,priority);
    return;
}

template <class T>
T priority_stack<T>::remove(T obj) {
    int priority = get_priority(obj);
    levels[priority]->erase(get_priorityID(obj));
    return;
}

template <class T>
T priority_stack<T>::walk() {
    /*
    if ( lvl_it == NULL ) {
        lvl_it = &(levels->begin());
        walk_it = &(*it->begin());
    } else if ( walk_it == NULL ) {
        walk_it = &(*(++lvl_it)->begin());
    }
    */
    return levels[0]->front();
}

#endif // ACTOR_MANAGER_H_INCLUDED
