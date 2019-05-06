#include "head.h"

void 		free_void_map(void **map, int size)
{
	int 	i;

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
		SDL_Quit();
		if (s->win)
			SDL_DestroyWindow(s->win);
		while (i < COUNT_TEXT && s->walls[i])
			SDL_FreeSurface(s->walls[i++]);
		if (s->path_map)
			ft_strdel(&s->path_map);
		free(s);
	}
//	ft_putendl("All was destroyed\nGood by :)");
//	system("leaks -q test_sdl_wolf");
	exit(1);
}

int					error_exit(char const *mess, t_sdl *s, t_game *g, t_player *p)
{
	if (mess && *mess)
	{
		ft_putstr("ERROR: ");
		ft_putendl(mess);
	}
	if (s)
		close_all(s);
	if (g)
		destroy_game(g);
	if (p)
		free(p);
	return (ERROR);
}

int					error_message(char const *mes)
{
	error_exit(mes, NULL, NULL, NULL);
	return (ERROR);
}
