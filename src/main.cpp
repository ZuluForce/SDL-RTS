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
#include "event_manager.h"
#include "ID.h"

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

typedef class {
    private:
        typedef struct {
            int maxFPS;
        } sSettings;

        string get_next_setting(string* filename);

} cSettings;

void onQuit(SDL_Event* event) {
    printf("Quit has been pressed!\n");
    quit_threads = true;
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
    SM.SM_set_caption("Planeman-RTS");
    SM.SM_maxFPS(20);
    SM_start(&SM);

    /* Setting up the event manager */
    cEvent_dispatch EM = cEvent_dispatch();
    EM.ED_reg_callback(SDL_QUIT,onQuit);

    cID_dispatch ID = cID_dispatch();
    printf("Current ID is %d\n",ID.ID_getid());
    printf("Current ID is %d\n",ID.ID_getid());
    printf("Current ID is %d\n",ID.ID_getid());
    ID.ID_returnid(2);
    printf("Current ID is %d\n",ID.ID_getid());
    printf("Current ID is %d\n",ID.ID_getid());

    while( true && !quit_threads) {
        EM.ED_manage_events(250);
        //SM.SM_set_caption("Planeman-RTS.");
        //SDL_Delay(500);
        //SM.SM_set_caption("Planeman-RTS..");
        //SDL_Delay(500);
        //SM.SM_set_caption("Planeman-RTS...");
        //SDL_Delay(500);
        SDL_Delay(10);
    }
    SM.cleanup(DEFAULT_TIMEOUT);
    SDL_Quit();
    return 0;
}
