#ifndef SIMPLE_ACTORS_H_INCLUDED
#define SIMPLE_ACTORS_H_INCLUDED

#include "actor_manager.h"
#include "std_gfx.h"

class Dot::cActor_static {
    private:
        sDisplay_info curr_info;
    public:
        Dot(int _typeID);
        bool check();
        sDisplay_info* get_display();
        int set_priority(int);
        bool set_image(char* filename);
        void set_pos(int x, int y);
}


#endif // SIMPLE_ACTORS_H_INCLUDED
