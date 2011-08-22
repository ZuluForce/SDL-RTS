#include "run_game.h"

extern cScreen_manager* pSM;
extern cPhysic_manager* pPM;
extern cEvent_dispatch* pEM;
extern cActor_manager* pAM;

extern bool quit_threads;
extern void (*onQuit) (SDL_Event*);

cGame::cGame() {
    init_resources();
    menu_callback = -1;
    game_thread_active = false;
}

cGame::~cGame() {
    printf("Cleaning up cGame\n");
    SDL_FreeSurface(G_dot_img);
    SDL_FreeSurface(G_back);
    SDL_FreeSurface(G_quit_button);
    game_thread_active = false;
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

    G_back = load_image("imgs\\back.bmp");
    G_quit_button = load_image("imgs\\quit_button.bmp");
    G_quit_clicked = load_image("imgs\\quit_button_clicked.bmp");
    G_quit_hover = load_image("imgs\\quit_button_hover.bmp");
}

void cGame::spawn_actor(void*) {
    Dot* obj1 = new Dot( 0 );
    obj1->set_image(G_dot_img);
    obj1->set_pos(225,179);
    obj1->priority = 0;
    pAM->AM_register(obj1);
}

void cGame::game_quit(int& button) {
    quit_threads = true;
    return;
}

int start_menu(void* args) {
    cGame* game = (cGame*) args;
    game->game_thread_active = true;

    std_menu* main_menu = new std_menu();
    main_menu->set_button_image(game->G_quit_button);
    main_menu->set_b_image_clicked(game->G_quit_clicked);
    main_menu->set_b_image_hover(game->G_quit_hover);
    main_menu->set_background(game->G_back);
    int quit;
    quit = main_menu->new_menu_button(0,0, MakeDelegate(game, &cGame::game_quit));
    main_menu->show_menu();

    while ( !quit_threads ) {
        switch ( game->menu_callback ) {
            case 0:
                game->menu_callback = -1;
                break;
            default:
                game->menu_callback = -1;
                break;
        }
        std_sleep(3);
    }
    printf("Exiting the game thread\n");
    return 0;
}
