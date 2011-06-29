#include "run_game.h"

void init_game_screen(cActor_manager* AM) {
    cID_dispatch type_ids = cID_dispatch();
    Dot* obj1 = new Dot( type_ids.ID_getid() );
    obj1->set_image("dot1.bmp");
    obj1->set_pos(100,100);

    AM->AM_register(obj1);
}
