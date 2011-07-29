#ifndef SIMPLE_ACTORS_H_INCLUDED
#define SIMPLE_ACTORS_H_INCLUDED

#include "actor_manager.h"
#include "std_gfx.h"
#include "physics.h"

extern pAM,pPM;

class Dot: public cActor {
    private:
        sDisplay_info* curr_info;
        vector<Uint8> _event_binds;

        phys_cont* p_container;

        int move_speed;
        int pressed_key[2];
    public:
        Dot(int _typeID);

        bool check();
        void check_events(event_vector**, int* load, Uint8* key_states);

        sDisplay_info* get_display();
        int set_priority(int);
        bool set_image(char* filename);
        bool set_image(SDL_Surface* image);
        void set_pos(int x, int y);
        vector<Uint8>* event_binds();
        SDL_Rect* get_clip();
};


#endif // SIMPLE_ACTORS_H_INCLUDED
