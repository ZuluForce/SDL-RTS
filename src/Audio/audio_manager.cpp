#include "Audio/audio_manager.h"

cAudio_manager::cAudio_manager() {
    if ( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1 ) {
        fprintf(stderr, "Failed to initialize SDL mixer: %s on line %d\n",__FILE__,__LINE__);
        return;
    }
    music = NULL;
}

cAudio_manager::~cAudio_manager() {
    if ( music != NULL ) Mix_FreeMusic( music );
    Mix_CloseAudio();
    return;
}

bool cAudio_manager::AMM_set_music(string music) {
    printf("Loading music file : %s\n",music);

    music = Mix_LoadMUS( music.c_str() );

    if ( music == NULL) {
        printf("/tFailed to load music file\n");
        return false;
    }
    return true;
}

void cAudio_manager::AM_play_music() {
    Mix_PlayMusic( music, -1);
}
