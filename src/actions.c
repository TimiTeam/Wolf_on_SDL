/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 18:43:09 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 17:36:59 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static void		rotate_player(t_player *p, t_cos_sin cos_sin)
{
	double		old_dir_x;
	double		old_plane_x;

	old_dir_x = p->dir.x;
	old_plane_x = p->plane.x;
	p->dir.x = p->dir.x * cos_sin.cos - p->dir.y * cos_sin.sin;
	p->dir.y = old_dir_x * cos_sin.sin + p->dir.y * cos_sin.cos;
	p->plane.x = p->plane.x * cos_sin.cos - p->plane.y * cos_sin.sin;
	p->plane.y = old_plane_x * cos_sin.sin + p->plane.y * cos_sin.cos;
}

static void		play_music(int *incr, Mix_Chunk *eff)
{
	Mix_PlayChannel(-1, eff, 0);
	*incr = 0;
}

static	int		key_up(SDL_Keycode k, t_sdl *s, t_player *p, t_game *g)
{
	int			x;
	int			y;

	if (k == SDLK_UP)
	{
		if (p->play == 5)
			play_music(&p->play, p->step);
		else
			p->play++;
		x = (int)(p->pos.x + p->dir.x * (p->speed + 0.1));
		y = (int)(p->pos.y + p->dir.y * (p->speed + 0.1));
		if (g->w_map[x][y] == 5)
		{
			Mix_PlayChannel(-1, p->door, 0);
			s->map++;
			return (NEW_GAME);
		}
		else if (g->w_map[x][y] == 13)
			return (THE_END);
		if (g->w_map[x][(int)p->pos.y] == 0)
			p->pos.x += p->dir.x * p->speed;
		if (g->w_map[(int)p->pos.x][y] == 0)
			p->pos.y += p->dir.y * p->speed;
	}
	return (0);
}

int				make_actions(SDL_Keycode k, t_sdl *s, t_player *p, t_game *g)
{
	if (k == SDLK_UP)
		return (key_up(k, s, p, g));
	else if (k == SDLK_DOWN)
	{
		if (p->play == 5)
		{
			Mix_PlayChannel(-1, p->step, 0);
			p->play = 0;
		}
		p->play++;
		if (g->w_map[(int)(p->pos.x - p->dir.x * p->speed)]
		[(int)p->pos.y] == 0)
			p->pos.x -= p->dir.x * p->speed;
		if (g->w_map[(int)p->pos.x][(int)
		(p->pos.y - p->dir.y * p->speed)] == 0)
			p->pos.y -= p->dir.y * p->speed;
	}
	else if (k == SDLK_LEFT)
		rotate_player(p, p->plus);
	else if (k == SDLK_RIGHT)
		rotate_player(p, p->minus);
	return (0);
}
