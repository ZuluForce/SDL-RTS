#include "run_game.h"

void init_game_screen(cActor_manager* AM) {
    cID_dispatch type_ids = cID_dispatch();
    Dot* obj1 = new Dot( type_ids.ID_getid() );
    obj1->set_image("dot.bmp");
    obj1->set_pos(100,100);
    obj1->priority = 0;
    AM->AM_register(obj1);

    for (int i = 0; i < 100; ++i) {
        obj1 = new Dot( type_ids.ID_getid() );
        obj1->set_image("dot.bmp");
        obj1->set_pos(100 + 4*i, 100);
        obj1->priority = 0;
        AM->AM_register(obj1);
    }
}
