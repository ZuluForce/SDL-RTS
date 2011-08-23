//#ifndef _cplusplus
//#error A C++ compiler is required!!
//#endif

#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include "SDL/SDL.h"
#include "std_gfx.h"
#include "screen_manager.h"
#include "actor_manager.h"
#include "event_manager.h"
#include "ID.h"
#include "run_game.h"
#include "UI/menu.h"

#ifdef __MINGW32__

#include  <windows.h>

#endif

#ifdef __GNUC__

#include <stdlib.h>

#endif


using namespace std;

/* Inserts symbol (as string) that is used
 * to delimit directories */

#define DELIMIT() str(\\)
#define str(s) #s

bool quit_threads = false;
//Will cause threads to clean up their space upon exit
//If only planning on pausing don't set to true
bool cleanup_threads = false;

/* Global Pointers */
//--These need to be defined for other managers to work--//
cEvent_dispatch* pEM;
cActor_manager* pAM;
cScreen_manager* pSM;
cPhysic_manager* pPM;

//------------------------------------------------------//

void onQuit(SDL_Event* event) {
    printf("Quit has been pressed!\n");
    quit_threads = true;
    return;
}

/* Main Purpose is for checking Escape */
void onKeyDown(SDL_Event* event) {
    switch( event->key.keysym.sym) {
        case SDLK_ESCAPE:
            quit_threads = true;
            break;
        default:
            break;
    }
    return;
}

int main(int argc, char** argv) {

    /* Chop executable filename off the directory argument */
    string cur_dir = argv[0];
    size_t index;
    index = cur_dir.find_last_of(DELIMIT());
    cur_dir = cur_dir.substr(0,index + 1);

    ofstream output ("out.txt", ios::out | ios::trunc);

    if (output.is_open()) {
        output << "The executable path is: " << argv[0] << "\n";
        output << "The currrent directory is: " << cur_dir << "\n";
        output.close();
    }

    /* Setting up the screen */
    cScreen_manager SM = cScreen_manager(640, 480, 32, SDL_SWSURFACE, true);
    SM.SM_set_caption("Planeman-Engine");
    SM.SM_maxFPS(2);
    SM_start(&SM);
    pSM = &SM;

    /* Setting up the Actor Manager */
    cActor_manager AM = cActor_manager(&SM);
    pAM = &AM;

    /* Setting up the event manager */
    cEvent_dispatch EM = cEvent_dispatch();
    EM.ED_reg_callback(SDL_QUIT,onQuit);
    EM.ED_reg_callback(ALL_EVENTS,AM_input_events);
    EM.ED_reg_callback(SDL_KEYDOWN,onKeyDown);
    pEM = &EM;

    /* Setting up the Physics Manager */
    cPhysic_manager PM = cPhysic_manager(3,3);
    pPM = &PM;

    /* Initializes the Actor Objects */
    //init_game_screen(&AM);
    cGame game_manager = cGame();
    SDL_CreateThread(start_menu,&game_manager);

    while( true && !quit_threads) {
        EM.ED_manage_events(250);
        AM.AM_update();
        std_sleep(2);
    }

    SM.cleanup(DEFAULT_TIMEOUT);
    game_manager.cleanup(DEFAULT_TIMEOUT);
    SDL_Quit();
    return 0;
}
