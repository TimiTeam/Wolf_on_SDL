#include "head.h"

void 				free_void_map(void **map, int size)
{
	int 			i;

	i = 0;
	while (i < size)
		free(map[i++]);
	free(map);
}

int 				destroy_game(t_game *g)
{
	if (g->w_map)
		free_void_map((void**)g->w_map, g->rows);
	if (g->elem)
		free(g->elem);
	free(g);
	return (OK);
}

int					close_all(t_sdl *s)
{
	int				i;

	i = 0;
	if (s)
	{
		if (s->win)
			SDL_DestroyWindow(s->win);
		if (s->img)
		{
			while (i < COUNT_TEXT && s->img->walls[i])
				SDL_FreeSurface(s->img->walls[i++]);
			free(s->img);
		}
		i = 0;
		while (i < 3)
			SDL_FreeSurface(s->menu->walls[i++]);
		i = 0;
		while (s->maps[i])
			ft_strdel(&s->maps[i++]);
		if (s->maps)
			free(s->maps);
		free(s->menu);
		free(s);
		SDL_Quit();
	}
	system("leaks -q test_sdl_wolf");
	return(1);
}

int					error_exit(char const *mess, t_sdl *s, t_player *p, t_game *g)
{
	if (mess && *mess)
	{
		ft_putstr("ERROR: ");
		ft_putendl(mess);
	}
	if (g)
		destroy_game(g);
	if (p)
		free(p);
	if (s)
		close_all(s);
	return (ERROR);
}

int					error_message(char const *mes)
{
	error_exit(mes, NULL, NULL, NULL);
	return (ERROR);
}