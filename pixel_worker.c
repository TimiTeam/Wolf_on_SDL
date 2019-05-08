#include "pixel_worker.h"

Uint32			get_pixel(SDL_Surface *src, int x, int y)
{
	int			bpp;
	Uint8		*p;

	bpp = src->format->BytesPerPixel;
	p = (Uint8*)src->pixels + y * src->pitch + x * bpp;
	switch (bpp)
	{
		case 1:
			return (*p);
		case 2:
			return (*(Uint16*)p);
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return (p[0] << 16 | p[1] << 8 | p[2]);
			else
				return (p[0] | p[1] << 8 | p[2] << 16);
		case 4:
			return (*(Uint32*)p);
		default:
			return (0);
	}
}

void			put_pixel(SDL_Surface *dst, int x, int y, Uint32 pixel)
{
	int			bpp;
	Uint8		*p;

	bpp = dst->format->BytesPerPixel;
	p = (Uint8*)dst->pixels + y * dst->pitch + x * bpp;
	switch (bpp)
	{
		case 1:
			*p = pixel;
			break ;
		case 2:
			*(Uint16*)p = pixel;
			break ;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break ;
		case 4:
			*(Uint32*)p = pixel;
			break ;
	default:
		break;
	}
}