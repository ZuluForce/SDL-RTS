#include "event_manager.h"

/* When a function is first to register itseld uncer a
 * particular event, space is allocated for future funcitons
 */
#define CALL_INIT_SIZE 20
#define RESIZE_FACTOR 2
#define DEFAULT_TIMEOUT 1000

#define LOAD call_loads[event][0]
#define _LOAD(x) call_loads[x][0]
#define MAX_LOAD call_loads[event][1]
#define CALL_PTR(event,num) callbacks[event][num]

//Need to fix up where call_init_size value is saved. Either macro or inside the class

cEvent_dispatch::cEvent_dispatch(int init_size, bool resize) {
    for (int i = 0; i < SDL_NUMEVENTS; ++i) {
        call_loads[i][0] = 0;
    }
    cEvent_dispatch::resize = resize;
    std_fuse manage_timeout(DEFAULT_TIMEOUT);
    return;
}

bool cEvent_dispatch::ED_reg_callback(Uint8 event, void (*call) (SDL_Event*)) {
    if (LOAD == 0) {
        /* Initialize */
        callbacks[event] = (void (**) (SDL_Event*)) malloc( CALL_INIT_SIZE * sizeof( void (*) () ) ) ;
        MAX_LOAD = CALL_INIT_SIZE;
    }
    else if ( LOAD == MAX_LOAD) {
        if ( resize ) {
            /* Resize */
            void (**temp) (SDL_Event*) = (void (**) (SDL_Event*)) malloc( (LOAD * RESIZE_FACTOR) * sizeof( void (*) () ) );
            MAX_LOAD = LOAD * RESIZE_FACTOR;

            for (int i = 0; i < LOAD; ++i) {
                temp[i] = callbacks[event][i];
            }

            free(callbacks[event]);
            callbacks[event] = temp;
        } else {
            /* ERROR!! */
            fprintf(stderr, "Resize is false, callback queue is full (event type: %d)", event);
            return false;
        }
    }

    /* Add the callback */
    callbacks[event][LOAD] = call;
    LOAD++;
    return true;
}

int cEvent_dispatch::ED_manage_events(int timeout) {
    manage_timeout->reset(true, timeout);

    SDL_Event event;

    while ( SDL_PollEvent(&event) && manage_timeout->check() ) {
        for (int i = 0; i < _LOAD(event.type); ++i) {
            CALL_PTR(event.type,i)(&event);

            if ( !manage_timeout->check() ) {
                if ( ++i == _LOAD(event.type)) return 1;

                saved_event = event;
                saved_callback = i;
                return -1;
            }
        }
    }
    return 0;
}
