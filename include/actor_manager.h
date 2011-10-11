#ifndef ACTOR_MANAGER_H_INCLUDED
#define ACTOR_MANAGER_H_INCLUDED

#include <vector>
#include <list>
#include <string>
#include <cstddef>
#include "SDL/SDL.h"
#include "screen_manager.h"
#include "physics.h"
#include "priority.h"
#include "ID.h"

using namespace std;

typedef vector<SDL_Event> event_vector;
typedef char* cstr;

struct sDisplay_info {
    SDL_Surface* surf;
    int x,y;
    SDL_Rect* clip;
};

class cActor {
    protected:
        sDisplay_info curr_info;

        vector<Uint8> _event_binds;
        bool update;

    public:
        int ID,typeID;
        int priority, priorityID;

        cActor();

        //#ifdef __MINGW32__ || __GNUC__
        virtual bool check();
        virtual void check_events(event_vector**, int* load, Uint8* key_states);
        virtual sDisplay_info* get_display();
        virtual int set_priority(int);
        virtual vector<Uint8>* event_binds();
        virtual cstr get_name();
        //#endif
};

/* Functions for mantaining order in the priority queue */

bool min_actor(void* actor1, void* actor2);

bool max_actor(void* actor1, void* actor2);

template <typename T>
class priority_stack {
    private:
        list<T>** levels;
        /* Iterator for the list we are in */
        typename list<T>::iterator lvl_it;
        /* Index of the list we are in */
        int lvl_index;
        /* Top Index in the array ocntaining the lists */
        int top_lvl; /* Top level actually containing an object */
        int low_lvl;
        int total_lvls;
        /* If the end of all lvls have been reached this will be true */
        bool end_walk;

        int (*get_priority) (T);
        int (*get_priorityID) (T);
        void (*mod_priorityID) (T,int);
        cstr (*get_name_string) (T);

        cID_dispatch priority_id_manager;

    public:
        /* This data type will not resize so allocate
           space for all the levels you may need up front */
        priority_stack (int init_size = 10);
        void reg_accessor(int (*foo) (T));
        void reg_IDaccessor(int (*foo) (T));
        void reg_IDmodifier(void (*foo) (T,int));
        void reg_getname(cstr (*foo) (T));

        void insert(T obj);
        T remove(T obj);
        list<T>* get_level(int level);

        T walk();
        void reset_walk();
};

/*------------------------------------------*/
class cActor_manager {
    private:
        cScreen_manager* SM;

        /* Screen Buffers */

        SDL_Surface* Draw_Buffer;
        SDL_Surface* Background;
        Uint32 Back_Color;
        /* 0 - No Permanent Background
           1 - Fill Color
           2 - SDL_Surface Backgorund
           */
        Uint8 back_type;

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
        void AM_set_bg(SDL_Color* fill_color = NULL);
        void AM_set_bg(SDL_Surface* fill_surf = NULL);
        void AM_blit_buffer(int x, int y, SDL_Surface* src, SDL_Rect* clip = NULL);
        void AM_blit_buffer(sDisplay_info*);
        void AM_blit_buffer(Uint32);
        void AM_flip_buffer();
        void AM_update();
        void AM_clear_load_buf();
        friend void AM_input_events(SDL_Event* event);
};

void AM_input_events(SDL_Event* event);
int Actor_Priority(cActor*);
int Actor_PriorityID(cActor*);
void Actor_modID(cActor*,int);
cstr Actor_name(cActor*);

/* Definitions of template functions */

template <class T>
priority_stack<T>::priority_stack(int init_size) {
    if ( init_size < 1 ) {
        fprintf(stderr,"Cannot create a priority_stack with no levels: init_size = %d\n",init_size);
    }
    levels = (list<T>** )malloc(init_size * sizeof( list<T>* ));
    for (int i = 0; i < init_size; ++i) {
        levels[i] = new list<T>;
    }

    priority_id_manager = cID_dispatch();
    lvl_it = levels[0]->begin();
    lvl_index = 0;
    top_lvl = 0;
    low_lvl = 0;
    total_lvls = init_size;
    end_walk = true;
    /* Initialize all the accessors and modifiers to null */
    get_priority = NULL;
    get_priorityID = NULL;
    mod_priorityID = NULL;
    get_name_string = NULL;
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
void priority_stack<T>::reg_getname(cstr (*foo) (T)) {
    get_name_string = foo;
    return;
}

template <class T>
void priority_stack<T>::insert(T obj) {
    int priority = get_priority(obj);

    if ( priority < 0 || priority > total_lvls) {
        if ( get_name_string ) {
            fprintf(stderr, "Failed to insert item : %s\n", get_name_string(obj));
        } else {
            fprintf(stderr, "Failed to insert item : Actor_Manager on line %d\n",__LINE__);
        }
        fprintf(stderr,"\tObj Address: %p\n",obj);
        fprintf(stderr,"\tAttempted lvl: %d   Max lvl: %d\n",priority,total_lvls);
        return;
    }

    mod_priorityID(obj,priority_id_manager.ID_getid());
    levels[priority]->push_back(obj);
    low_lvl = priority < low_lvl ? priority : low_lvl;
    top_lvl = priority > top_lvl ? priority : top_lvl;
    end_walk = false;
    printf("Priority Stack inserted object with address %p at lvl (%d)\n\n",obj,priority);
    return;
}

template <class T>
T priority_stack<T>::remove(T obj) {
    int priority = get_priority(obj);
    levels[priority]->erase(get_priorityID(obj));
    return;
}

template <class T>
list<T>* priority_stack<T>::get_level(int level) {
    return levels[level];
}

template <class T>
T priority_stack<T>::walk() {
    if ( end_walk ) return NULL;
    T ret_val;

    if ( lvl_it == levels[lvl_index]->end() ) {
        /* We have reached the very end of the walk */
        if ( lvl_index >= top_lvl ) {
            end_walk = true;
            ret_val = NULL;
            return ret_val;
        }
        /* Find the next non-empty level or stop if end is reached */
        while ( levels[++lvl_index]->empty()) {
            if ( lvl_index >= top_lvl ) {
                end_walk = true;
                ret_val = NULL;
                return ret_val;
            }
        }
        /* Assign the iterator to the new level */
        lvl_it = levels[lvl_index]->begin();
    }

    ret_val = *lvl_it;
    ++lvl_it;

    return ret_val;
    //return levels[0]->front();
}

template<class T>
void priority_stack<T>::reset_walk() {
    lvl_index = low_lvl;
    end_walk = false;
    lvl_it = levels[lvl_index]->begin();
}

#endif // ACTOR_MANAGER_H_INCLUDED
