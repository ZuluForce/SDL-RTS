#ifndef EVENT_MANAGER_H_INCLUDED
#define EVENT_MANAGER_H_INCLUDED

#include "SDL/SDL.h"
#include "std_gfx.h"

#define CALL_INIT_SIZE 20
#define DEFAULT_TIMEOUT 1000
#define CALL_HEADER void (*) (SDL_Event*)
#define _CALL_HEADER(name, arg) void(*name) (SDL_Event* arg)

#define ALL_EVENTS 0

typedef class cEvent_dispatch {
    private:
        bool resize;

        void (**callbacks[SDL_NUMEVENTS]) (SDL_Event* event);

        /* [Current load, Max load] */
        int call_loads[SDL_NUMEVENTS][2];

        /* In the case that a timeout occurs while doing callbacks
         * the location of the next callback funciton and event will
         * be saved in this buffer */
        SDL_Event saved_event;
        int saved_callback;

        std_fuse* manage_timeout;

    public:
        cEvent_dispatch(int init_size = CALL_INIT_SIZE, bool resize = false);
        bool ED_reg_callback(Uint8 event, void (*call) (SDL_Event*));

        /* Default timeout of 1 second to prevent extended lockups */
        /* Return Values:
         * -1: Exited due to timeout
         * 0 : All events were handled and there is more time left
         * 1 : All events were handled but time ran out
         */
        int ED_manage_events(int timeout = DEFAULT_TIMEOUT);

} cEvent_dispatch;

#endif // EVENT_MANAGER_H_INCLUDED
