#ifndef	__PIXEL_WORKER_H
#define	__PIXEL_WORKER_H

#include <SDL.h>

Uint32			get_pixel(SDL_Surface *src, int x, int y);
void			put_pixel(SDL_Surface *dst, int x, int y, Uint32 pixel);

#endif