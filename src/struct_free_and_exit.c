/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_free_and_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:10:44 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 18:11:41 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void		clear_image(t_images *im)
{
	int		i;

	if (!im)
		return ;
	i = 0;
	while (im->walls[i])
		SDL_FreeSurface(im->walls[i++]);
	if (im->surf)
		SDL_FreeSurface(im->surf);
	free(im);
}

int			destroy_game(t_game *g)
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

int			close_all(t_sdl *s)
{
	int		i;

	i = 0;
	SDL_DestroyRenderer(s->ren);
	SDL_DestroyWindow(s->win);
	clear_image(s->img);
	clear_image(s->menu);
	if (s->maps)
	{
		while (s->maps[i])
			ft_strdel(&s->maps[i++]);
		free(s->maps);
	}
	if (s->music)
		Mix_FreeMusic(s->music);
	if (s->menu_move)
		Mix_FreeChunk(s->menu_move);
	if (s->menu_select)
		Mix_FreeChunk(s->menu_select);
	Mix_Quit();
	SDL_Quit();
	ft_memdel((void**)&s);
	return (1);
}

int			exit_x(t_sdl *s, t_player *p, t_game *g)
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

int			error_message(char const *m, t_sdl *s, t_player *p, t_game *g)
{
	if (m)
	{
		ft_putstr("ERROR: ");
		ft_putendl(m);
	}
	return (exit_x(s, p, g));
}
