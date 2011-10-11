#include "run_game.h"

extern cScreen_manager* pSM;
extern cPhysic_manager* pPM;
extern cEvent_dispatch* pEM;
extern cActor_manager* pAM;

extern bool quit_threads;
extern void (*onQuit) (SDL_Event*);

cAudio_manager* pAMM;

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
    G_dot_img = load_image("imgs/dot.bmp");
    SDL_SetColorKey(G_dot_img, SDL_SRCCOLORKEY | SDL_RLEACCEL,0);

    G_menu_back = load_image("imgs/menu_back.png");
    G_game_back = load_image("imgs/back.png");
    G_start_button = load_image("imgs/start_button.png");
    G_start_hover = load_image("imgs/start_button_hover.png");
    G_settings_button = load_image("imgs/settings_button.png");
    G_settings_hover = load_image("imgs/settings_button_hover.png");
    G_quit_button = load_image("imgs/quit_button.png");
    G_quit_clicked = load_image("imgs/quit_button_clicked.png");
    G_quit_hover = load_image("imgs/quit_button_hover.png");
    G_mute_button = load_png("imgs/mute.png");
    G_muted = load_png("imgs/muted.png");
    G_scale = load_png("imgs/scale.png");
    G_scale_load = load_png("imgs/scale_load.png");
    G_scale_slide = load_png("imgs/scale_slide.png");
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
    /* Quit Button */
    menu_button* new_button = new menu_button(270,220,G_quit_button,G_quit_hover,G_quit_clicked);
    b_quit = main_menu->reg_menu_obj(new_button, MakeDelegate(this,&cGame::game_quit));

    /* Start Button */
    new_button = new menu_button(270,100,G_start_button,G_start_hover,G_quit_clicked);
    b_start = main_menu->reg_menu_obj(new_button, MakeDelegate(this, &cGame::start_game));

    /* Settings Button */
    new_button = new menu_button(270,160,G_settings_button,G_settings_hover,G_quit_clicked);
    b_settings = main_menu->reg_menu_obj(new_button, MakeDelegate(this, &cGame::load_settings));

    /* Mute Button */
    new_button = new mute_button(0,0,G_mute_button,G_mute_button,G_muted);
    b_mute = main_menu->reg_menu_obj(new_button, MakeDelegate(AMM, &cAudio_manager::AMM_set_music_vol));
    main_menu->show_menu(b_quit,b_mute);

    menu_slider* new_slider = new menu_slider(270,220,G_scale,G_scale_load,G_scale_slide);
    new_slider->set_style(true,0);
    new_slider->set_return_val(0,MIX_MAX_VOLUME);
    new_slider->set_slider_pos(100);
    b_music_vol = main_menu->reg_menu_obj(new_slider, MakeDelegate(AMM, &cAudio_manager::AMM_set_music_vol));

	/* Back Button */
    back_button* back = new back_button(270,300,G_settings_button,G_settings_hover,G_quit_clicked);
    b_back = main_menu->reg_menu_obj(back, MakeDelegate(main_menu, &std_menu::dummy_callback));
    back->set_menu_list(main_menu->get_menu_state(), true);
    back->reg_callback( MakeDelegate(main_menu, &std_menu::set_back));
}

void cGame::start_game(int& button) {
    //Hide the start menu
    main_menu->hide_menu();
    pAM->AM_set_bg(G_game_back);

    /* Currently there is a problem with a xcb_io library in
       Ubuntu 11.04 that is not allowing the game to work if
       a Dot actor is created */
    #ifdef __MINGW32__
    Dot* new_dot = new Dot(0);
    new_dot->set_pos(0,0);
    new_dot->set_image(G_dot_img);
    new_dot->set_priority(3);
    pAM->AM_register(new_dot);
    this->active_objs.push_back(new_dot);
    #endif

    params* temp_param = new params;
    temp_param->w_h = new coordinates;
    temp_param->w_h->first = 100;
    temp_param->w_h->second = 100;

    pPM->PM_set_collide_zone(200, 200, temp_param, 0);
}

void cGame::load_settings(int& button) {
    main_menu->hide_menu(b_quit,b_settings);
    main_menu->show_menu(b_music_vol,b_back);
}

void cGame::game_quit(int& button) {
    quit_threads = true;
    return;
}

int start_menu(void* args) {
    cGame* game = (cGame*) args;
    cAudio_manager* AMM = new cAudio_manager();
    game->AMM = AMM;
    pAMM = AMM;
    game->game_thread_active = true;

    std_menu* main_menu = new std_menu();
    game->main_menu = main_menu;
    game->start_menu();
    AMM->AMM_set_music("audio/menu_music.mp3");
    //AMM->AMM_set_music("audio/Granite.flac");
    AMM->AMM_play_music();

    while ( !quit_threads ) {
        game->update();
        std_sleep(20);
    }

    printf("Exiting the game thread\n");
    game->game_thread_active = false;
    return 0;
}
