#include "head.h"

void 		free_void_map(void **map, int size)
{
	int 	i;

	i = 0;
	while (i < size)
		free(map[i++]);
	free(map);
}

int					clear_fields_t_game(t_game *g)
{
	g->m.x = 0;
	g->m.y = 0;
	g->step.x = 0;
	g->step.y = 0;
	g->side = 0;
	g->wall_dist = 0;
	g->wall_size = 0;
	g->half_wall_size = 0;
	g->side_dist.x = 0;
	g->side_dist.y = 0;
	g->delta_dist.x = 0;
	g->delta_dist.y = 0;
	return (OK);
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
		while (i < COUNT_TEXT)
			SDL_FreeSurface(s->walls[i++]);
		if (s->path_map)
			ft_strdel(&s->path_map);
		free(s);
	}
	SDL_Quit();
	ft_putendl("All was destroyed\nGood by :)");
//	system("leaks -q test_sdl_wolf");
	return (0);
}

int					error_exit(char const *mess, t_sdl *s, t_game *g)
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
	return (ERROR);
}

int					error_message(char const *mes)
{
	error_exit(mes, NULL, NULL);
	return (ERROR);
}
