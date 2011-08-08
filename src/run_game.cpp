#include "run_game.h"

void init_game_screen(cActor_manager* AM) {
    SDL_Surface* dot_image = load_image("dot.bmp");
    SDL_SetColorKey(dot_image, SDL_SRCCOLORKEY | SDL_RLEACCEL,0);

    cID_dispatch type_ids = cID_dispatch();
    Dot* obj1 = new Dot( type_ids.ID_getid() );
    obj1->set_image(dot_image);
    obj1->set_pos(0,0);
    obj1->priority = 0;
    AM->AM_register(obj1);

/*
    for (int i = 0; i < 100; ++i) {
        obj1 = new Dot( type_ids.ID_getid() );
        obj1->set_image(dot_image);
        obj1->set_pos(100 + 4*i, 100);
        obj1->priority = 0;
        AM->AM_register(obj1);
    }
    */

    obj1 = new Dot( type_ids.ID_getid() );
    obj1->set_image(dot_image);
    obj1->set_pos(100, 100);
    obj1->priority = 1;
    AM->AM_register(obj1);
    return;
}
