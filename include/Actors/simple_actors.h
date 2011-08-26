#ifndef SIMPLE_ACTORS_H_INCLUDED
#define SIMPLE_ACTORS_H_INCLUDED

#include "actor_manager.h"
#include "std_gfx.h"
#include "physics.h"

class Dot: public cActor {
    private:
        vector<Uint8> _event_binds;

        phys_cont* p_container;

        int move_speed;
        int pressed_key[2];
        bool wasd;
    public:
        Dot(int _typeID);

        bool check();
        void check_events(event_vector**, int* load, Uint8* key_states);

        sDisplay_info* get_display();
        int set_priority(int);
        bool set_image(char* filename);
        bool set_image(SDL_Surface* image);
        void set_pos(int x, int y);
        void change_control();
        vector<Uint8>* event_binds();
        SDL_Rect* get_clip();
};

/* Doesn't do anything but you can move it */
class static_obj : public cActor {
    public:
        static_obj(int x, int y, SDL_Surface* surf);
        void move_to(int x, int y);
        void move_delta(int x, int y);

        void set_clip(SDL_Rect* clip) {
            curr_info.clip = clip;
        }
};


#endif // SIMPLE_ACTORS_H_INCLUDED
