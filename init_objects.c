#include "head.h"


void 			find_free_place(t_game *s, t_vec *pos)
{
	int 		i;
	int 		j;

	i = 0;
	while (i < s->rows)
	{
		j = 0;
		while (j < s->elem[i])
		{
			if (s->w_map[i][j] == 0 )
			{
				pos->x = i + 0.2;
				pos->y = j + 0.2;
				return ;
			}
			j++;
		}
		i++;
	}
}

int				init_objects(t_sdl *s, t_player **p, t_game **g)
{
	t_vec		pos;

	*p = NULL;
	*g = NULL;	
	if (init_sdl_elem(s) == ERROR)
		return (error_message(SDL_GetError()));
	if(!(*g = create_game(s->path_map)))
		return (error_message("Failed to create game :("));
	if (!(*p = create_player()))
		return (error_message("Create player"));
	find_free_place(*g, &pos);
	(*p)->pos = pos;
	return (OK);
}

int				init_sdl_elem(t_sdl *s)
{
	int			flag;

	flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (ERROR);
	s->win = SDL_CreateWindow("test_sdl_wolf", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, s->win_size.x, s->win_size.y, SDL_WINDOW_SHOWN);
	if (!s->win)
		return (ERROR);
	s->ren = SDL_CreateRenderer(s->win, -1, flag);
	if (!s->ren)
		return (ERROR);
	if (!(s->img = create_and_images("res/resurses.txt", s->ren)))
		return (ERROR);
	return (0);
}