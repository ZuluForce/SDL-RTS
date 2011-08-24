#ifndef AUDIO_MANAGER_H_INCLUDED
#define AUDIO_MANAGER_H_INCLUDED

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

/* Abreviated AMM for future refernece.
   Stands for Audio/Music manager.
   Trying to avoid conflict with Actor Manager */
class cAudio_manager {
    private:
        Mix_Music* music;

    public:
        cAudio_manager();
        ~cAudio_manager();
        bool AMM_set_music(std::string);
        void AMM_play_music();
        void AMM_set_music_vol(int& volume);
        int AMM_get_music_vol();
};

#endif //AUDIO_MANAGER_H_INCLUDED
