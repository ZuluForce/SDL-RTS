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

typedef class {
    private:
        typedef struct {
            int maxFPS;
        } sSettings;

        string get_next_setting(string* filename);

} cSettings;

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

    cScreen_manager SM = cScreen_manager(640, 480, 32, SDL_SWSURFACE, true);
    SM.SM_set_caption("Planeman-RTS");

    Sleep(10000);

    return 0;
}
