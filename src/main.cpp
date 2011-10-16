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
#include "Settings/iniReader.h"

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
INIReader* pSettings;

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

void tune_update(int& sleeptime) {
    if ( pSM->SM_backlog(3) ) {
        printf("Adjusting sleeptime to %d milliseconds\n", sleeptime * 2);
        sleeptime *= 2;
        return;
    } else if ( pSM->SM_backlog(2) ) {
        printf("Adjusting sleeptime to %d milliseconds\n", sleeptime + 1);
        ++sleeptime;
        return;
    }
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

    /* Checking for settings */
    INIReader* game_settings = new INIReader("settings/engine_settings.ini");
    if ( !game_settings->loaded() ) {
        delete(game_settings);
        game_settings = NULL;
        pSettings = NULL;
    } else {
        pSettings = game_settings;
    }
    /* Setting up the screen */
    cScreen_manager SM = cScreen_manager(640, 480, 32, SDL_SWSURFACE, true);
    SM.SM_set_caption("Planeman-Engine");
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
    cGame game_manager = cGame(game_settings);
    SDL_CreateThread(start_menu,&game_manager);

    int update_rate;
    bool tune;
    if ( pSettings ) {
        if ( pSettings->exists("Core", "loop_rate") ) {
            update_rate = 1000 / pSettings->extractValue<int>("Core", "loop_rate");
        } else {
            update_rate = 10;
        }

        if ( pSettings->exists("Core", "tune_rate") ) {
            tune = pSettings->extractValue<bool>("Core", "tune_rate");
        } else {
            tune = true;
        }
    }

	std_fuse main_fuse = std_fuse();

    while( true && !quit_threads) {
    	main_fuse.start(update_rate);
        EM.ED_manage_events(250);
        AM.AM_update();

        if ( tune ) {
            tune_update(update_rate);
        }

        main_fuse.wait_out();
    }

    SM.cleanup(DEFAULT_TIMEOUT);
    game_manager.cleanup(DEFAULT_TIMEOUT);
    SDL_Quit();
    return 0;
}
