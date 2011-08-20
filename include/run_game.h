#ifndef RUN_GAME_H_INCLUDED
#define RUN_GAME_H_INCLUDED

#include <cstdio>
#include "actor_manager.h"
#include "std_gfx.h"
#include "Actors/simple_actors.h"
#include "physics.h"
#include "UI/menu.h"

void init_game_screen(cActor_manager* AM);

int start_menu(void*);


#endif // RUN_GAME_H_INCLUDED
