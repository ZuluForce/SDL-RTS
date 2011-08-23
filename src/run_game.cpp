#include "run_game.h"

extern cScreen_manager* pSM;
extern cPhysic_manager* pPM;
extern cEvent_dispatch* pEM;
extern cActor_manager* pAM;

extern bool quit_threads;
extern void (*onQuit) (SDL_Event*);

cGame::cGame() {
    init_resources();
    game_thread_active = false;
}

cGame::~cGame() {
    printf("Cleaning up cGame\n");
    SDL_FreeSurface(G_dot_img);
    SDL_FreeSurface(G_game_back);
    SDL_FreeSurface(G_menu_back);
    SDL_FreeSurface(G_quit_button);
    game_thread_active = false;
}

void cGame::update() {
    return;
}

void cGame::cleanup(int timeout) {
    std_fuse cleanupFuse = std_fuse();
    cleanupFuse.start( timeout );

    while ( game_thread_active ) {
        if ( !cleanupFuse.check() ) {
            fprintf(stderr, "Timeout while waiting to cleanup the Game Manager.\n");
            SDL_KillThread( game_thread_ptr );
            game_thread_active = false;
            return;
        }
        std_sleep(5);
    }
    game_thread_active = false;
    return;
}

void cGame::init_resources() {
    G_dot_img = load_image("imgs\\dot.bmp");
    SDL_SetColorKey(G_dot_img, SDL_SRCCOLORKEY | SDL_RLEACCEL,0);

    G_menu_back = load_image("imgs\\menu_back.png");
    G_game_back = load_image("imgs\\back.bmp");
    G_start_button = load_image("imgs\\start_button.png");
    G_quit_button = load_image("imgs\\quit_button.png");
    G_quit_clicked = load_image("imgs\\quit_button_clicked.png");
    G_quit_hover = load_image("imgs\\quit_button_hover.png");
}

void cGame::spawn_actor(void*) {
    Dot* obj1 = new Dot( 0 );
    obj1->set_image(G_dot_img);
    obj1->set_pos(225,179);
    obj1->priority = 0;
    pAM->AM_register(obj1);
}

void cGame::start_menu() {
    main_menu->set_button_image(G_quit_button);
    main_menu->set_b_image_clicked(G_quit_clicked);
    main_menu->set_b_image_hover(G_quit_hover);
    main_menu->set_background(G_menu_back);
    b_quit = main_menu->new_menu_button(270,220, MakeDelegate(this, &cGame::game_quit));
    main_menu->set_button_image(G_start_button);
    b_start = main_menu->new_menu_button(270,160, MakeDelegate(this, &cGame::start_game));
    main_menu->show_menu();
}

void cGame::start_game(int& button) {
    //Hide the start menu
    main_menu->hide_menu();
    pAM->AM_set_bg(G_game_back);

    Dot* new_dot = new Dot(0);
    new_dot->set_pos(0,0);
    new_dot->set_image(G_dot_img);
    new_dot->set_priority(1);
    pAM->AM_register(new_dot);
    this->active_objs.push_back(new_dot);

    params* temp_param = new params;
    temp_param->w_h = new coordinates;
    temp_param->w_h->first = 100;
    temp_param->w_h->second = 100;

    pPM->PM_set_collide_zone(200, 200, temp_param, 0);
}

void cGame::game_quit(int& button) {
    quit_threads = true;
    return;
}

int start_menu(void* args) {
    cGame* game = (cGame*) args;
    game->game_thread_active = true;

    std_menu* main_menu = new std_menu();
    game->main_menu = main_menu;
    game->start_menu();

    /*
    main_menu->set_button_image(game->G_quit_button);
    main_menu->set_b_image_clicked(game->G_quit_clicked);
    main_menu->set_b_image_hover(game->G_quit_hover);
    main_menu->set_background(game->G_back);
    int quit;
    quit = main_menu->new_menu_button(300,220, MakeDelegate(game, &cGame::game_quit));
    main_menu->show_menu();
    */

    while ( !quit_threads ) {
        game->update();
        std_sleep(3);
    }
    printf("Exiting the game thread\n");
    game->game_thread_active = false;
    return 0;
}
