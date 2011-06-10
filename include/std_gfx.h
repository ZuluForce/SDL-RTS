#ifndef STD_GFX_H_INCLUDED
#define STD_GFX_H_INCLUDED

#include "SDL/SDL.h"
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
SDL_Surface *load_image(const char* filename );

#endif //STD_GFX_H_INCLUDED
