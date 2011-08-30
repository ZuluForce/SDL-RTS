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

bool cAudio_manager::AMM_set_music(std::string music_file) {
    printf("Loading music file : %s\n", music_file.c_str());

    music = Mix_LoadMUS( music_file.c_str() );

    if ( music == NULL) {
        printf("\tFailed to load music file\n");
        fprintf(stderr, "Failed to load music file : %s\n",music_file.c_str());
        return false;
    }
    return true;
}

void cAudio_manager::AMM_play_music() {
    Mix_PlayMusic( music, -1);
}

void cAudio_manager::AMM_set_music_vol(int& vol) {
    printf("Setting volume to: %d\n",vol);
    Mix_VolumeMusic(vol);
}

int cAudio_manager::AMM_get_music_vol() {
    return Mix_VolumeMusic(-1);
}
