/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_free_and_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:10:44 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/17 18:23:46 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void			free_void_map(void **map, int size)
{
	int			i;

	i = 0;
	while (i < size)
	{
		free(map[i]);
		map[i++] = NULL;
	}
	free(map);
	map[i] = NULL;
}

int				destroy_game(t_game *g)
{
	if (g->w_map)
		free_void_map((void**)g->w_map, g->rows);
	if (g->elem)
		free(g->elem);
	g->elem = NULL;
	free(g);
	g = NULL;
	return (OK);
}

int				close_all(t_sdl *s)
{
	int			i;

	i = 0;
	SDL_DestroyRenderer(s->ren);
	SDL_DestroyWindow(s->win);
	if (s->img)
	{
		while (i < CO_TEXT && s->img->walls[i])
			SDL_FreeSurface(s->img->walls[i++]);
		SDL_FreeSurface(s->img->surf);
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
	if (s->music)
		Mix_FreeMusic(s->music);
	if (s->menu_move)
		Mix_FreeChunk(s->menu_move);
	if (s->menu_select)
		Mix_FreeChunk(s->menu_select);
	free(s);
	Mix_Quit();
	SDL_Quit();
	system("leaks -q wolf3d");
	return (1);
}

int				exit_x(t_sdl *s, t_player *p, t_game *g)
{
	if (g)
		destroy_game(g);
	if (p)
	{
		if (p->step)
			Mix_FreeChunk(p->step);
		if (p->door)
			Mix_FreeChunk(p->door);
		free(p);
	}
	if (s)
		close_all(s);
	return (ERROR);
}

int				error_message(char const *mess, t_sdl *s, t_player *p, t_game *g)
{
	if (mess)
	{
		ft_putstr("ERROR: ");
		ft_putendl(mess);
	}
	exit_x(s, p, g);
	return (ERROR);
}
