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
    private:
        //Lock the objects position on a particular axis
        bool x_lock,y_lock;
    public:
        static_obj(int x, int y, SDL_Surface* surf);

        void lock_on_x();
        void lock_on_y();
        void lock_xy();

        void move_to(int x, int y);
        void move_delta(int x, int y);

        void show();
        void hide();

        void set_clip(SDL_Rect* clip) {
            curr_info.clip = clip;
        }

        char* get_name();
};


#endif // SIMPLE_ACTORS_H_INCLUDED
