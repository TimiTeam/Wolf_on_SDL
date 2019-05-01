#include "head.h"

void 		free_void_map(void **map, int size)
{
	int 	i;

	i = 0;
	while (i < size)
		free(map[i++]);
	free(map);
}


int					close_all(t_sdl *s)
{
	int				i;

	i = 0;
	if (s)
	{
		if (s->win)
			SDL_DestroyWindow(s->win);
		if (s->ren)
			SDL_DestroyRenderer(s->ren);
	//	if (s->surf)
	//		SDL_FreeSurface(s->surf);
/*		if (s->tex[0])
			while (i < COUNT_TEXT)
				SDL_DestroyTexture(s->tex[i++]);*/
		free (s);
	/*	if (s->w_map)
			free_void_map((void**)s->w_map, s->rows);*/
	/*	if (s->elem)
			free(s->elem);*/
	}
	SDL_Quit();
	ft_putendl("All was destroyed\nGood by :)");
	//system("leaks test_sdl_wolf");
	return (0);
}

int					error_exit(char const *mess, t_sdl *s)
{
	if (mess && *mess)
	{
		ft_putstr("Fail: ");
		ft_putendl(mess);
	}
	if (s)
		close_all(s);
	return (ERROR);
}

int					error_message(char const *mes)
{
	error_exit(mes, NULL);
	return (ERROR);
}
