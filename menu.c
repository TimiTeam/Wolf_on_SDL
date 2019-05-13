#include "head.h"

static int				    copy_surf(SDL_Surface *dst, t_point to, SDL_Surface *src, t_point from)
{
	Uint32			        pixel;
	int				        y_to;
	int				        y_from;

	if (!dst || !src)
		return (1);
	while (to.x < dst->w && from.x < src->w)
	{
		y_to = to.y;
		y_from = from.y;
		while (y_to < dst->h && y_from < src->h)
		{
			pixel = get_pixel(src, from.x, y_from);
			put_pixel(dst, to.x, y_to, pixel);
			y_from++;
			y_to++;
		}
		to.x++;
		from.x++;
	}
	return (0);
}

static SDL_Surface			*make_menu_surf(SDL_Surface *choice)
{
	SDL_Surface 	        *ret;
	t_point			        from;

	if (!choice)
		return (NULL);
	ret = SDL_CreateRGBSurface(0, choice->w, choice->h, 32, 0, 0 ,0, 255);
	if (!ret)
		return (NULL);
	from.x = 0;
	from.y = 0;
	copy_surf(ret, from, choice, from);
	return (ret);
}

void                        render_manipulations(SDL_Renderer *ren, SDL_Texture *tex)
{
    SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);
	SDL_DestroyTexture(tex);
}

int					        show_menu(t_sdl *s)
{

	SDL_Event	            e;
	SDL_Texture	            *tex;
	int			            choice;
	
	choice = 0;
	while (1)
	{
		s->menu->surf = make_menu_surf(s->menu->walls[choice]);
		tex = SDL_CreateTextureFromSurface(s->ren, s->menu->surf);
        render_manipulations(s->ren, tex);
		SDL_FreeSurface(s->menu->surf);
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				return (OK);
			else if (e.type == SDL_KEYDOWN)
			{
				switch(e.key.keysym.sym)
				{
					case (SDLK_UP):
						--choice;
						if (choice < 0)
							choice = 2;
						break ;
					case (SDLK_DOWN):
						++choice;
						if (choice > 2)
							choice = 0;
						break ;
					case (SDLK_RETURN):
						if (choice == 2)
							return (OK);
						else if (choice == 1)
							return (CONTINUE);
						else if (choice == 0)
						{
							s->map = 0;
							return (NEW_GAME);
						}
					case (SDLK_ESCAPE):
						return (OK);
				}
			}
		}
	}
}