#include "run_game.h"

extern cScreen_manager* pSM;
extern cPhysic_manager* pPM;
extern cEvent_dispatch* pEM;
extern cActor_manager* pAM;

extern bool quit_threads;
extern void (*onQuit) (SDL_Event*);

void init_game_screen(cActor_manager* AM) {
    SDL_Surface* dot_image = load_image("imgs\\dot.bmp");
    SDL_Surface* back = load_image("imgs\\back.bmp");
    SDL_SetColorKey(dot_image, SDL_SRCCOLORKEY | SDL_RLEACCEL,0);
    AM->AM_set_bg(back);

    cID_dispatch type_ids = cID_dispatch();
    Dot* obj1 = new Dot( type_ids.ID_getid() );
    obj1->set_image(dot_image);
    obj1->set_pos(225,179);
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
    obj1->set_pos(200,0);
    obj1->priority = 1;
    obj1->change_control();
    AM->AM_register(obj1);

    params left_out;
    left_out.w_h = new coordinates;
    left_out.w_h->first = 100;
    left_out.w_h->second = 100;
    pPM->PM_set_collide_zone(200,200,&left_out);

    /*
    left_out.w_h->first = pSM->SM_get_w();
    left_out.w_h->second = 0;
    pPM->PM_set_collide_zone(0,pSM->SM_get_h(),&left_out);
    */

    /* Test of line collision algorithm */
    line line1,line2;
    int i = 1;
    //Two slopping lines
    line1 = {0,0,4,4};
    line2 = {0,4,4,0};
    printf("Line test %d (Expected: %s)\n",i++,"True");
    printf("\tResult: %s\n\n",pPM->PM_call_check(line1,line2) ? "True" : "False");
    //One vertical one not
    line1 = {3,5,3,-5};
    line2 = {-2,4,6,4};
    printf("Line test %d (Expected: %s)\n",i++,"True");
    printf("\tResult: %s\n\n",pPM->PM_call_check(line1,line2) ? "True" : "False");
    //Two Vertical Lines
    line2 = {-2,4,-2,-4};
    printf("Line test %d (Expected: %s)\n",i++,"False");
    printf("\tResult: %s\n\n",pPM->PM_call_check(line1,line2) ? "True" : "False");
    //Two parallel lines
    line1 = {0,4,2,0};
    line2 = {0,0,-1,-2};
    printf("Line test %d (Expected: %s)\n",i++,"False");
    printf("\tResult: %s\n\n",pPM->PM_call_check(line1,line2) ? "True" : "False");
    //One horizontal, one slopping
    line2 = {-2,3,6,3};
    printf("Line test %d (Expected: %s)\n",i++,"True");
    printf("\tResult: %s\n\n",pPM->PM_call_check(line1,line2) ? "True" : "False");
    //Debugging a failing test from physics.cpp
    line1 = {301,225,229,225};
    line2 = {300,200,300,300};
    printf("Line test %d (Expected: %s)\n",i++,"True");
    printf("\tResult: %s\n\n",pPM->PM_call_check(line1,line2) ? "True" : "False");
    return;
}

int start_menu(void* args) {
    std_menu* menu_sys = new std_menu();
    SDL_Surface* quit_button = load_image("imgs\\quit_button.bmp");
    SDL_Surface* background = load_image("imgs\\back.bmp");
    menu_sys->set_button_image(quit_button);
    menu_sys->set_background(background);
    int quit;
    quit = menu_sys->new_menu_button(0,0);
    menu_sys->show_menu();

    while ( !quit_threads ) {
        pEM->ED_manage_events(250);
        pAM->AM_update();
        std_sleep(3);
    }
    return 0;
}
