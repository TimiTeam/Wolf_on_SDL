/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_wall.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 19:07:12 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 19:09:16 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static void		calculate_strip_wall(t_player *p, t_game *game, t_data *d)
{
	while (1)
	{
		if (d->side_dist.x < d->side_dist.y)
		{
			d->side_dist.x += d->delta_dist.x;
			d->move.x += d->step.x;
			d->side = 0;
		}
		else
		{
			d->side_dist.y += d->delta_dist.y;
			d->move.y += d->step.y;
			d->side = 1;
		}
		if (game->w_map[d->move.x][d->move.y] > 0)
			break ;
	}
	if (d->side)
		d->wall_dist = (d->move.y - p->pos.y +
				(double)(1 - d->step.y) / 2) / d->ray.y;
	else
		d->wall_dist = (d->move.x - p->pos.x +
				(double)(1 - d->step.x) / 2) / d->ray.x;
	d->wall_dist = d->wall_dist <= 0 ? 1.0 : d->wall_dist;
}

static void		calculate_side_dist(t_data *d, t_player *player)
{
	if (d->ray.x < 0)
	{
		d->step.x = -1;
		d->side_dist.x = (player->pos.x - d->move.x) * d->delta_dist.x;
	}
	else
	{
		d->step.x = 1;
		d->side_dist.x = (d->move.x + 1.0 - player->pos.x) * d->delta_dist.x;
	}
	if (d->ray.y < 0)
	{
		d->step.y = -1;
		d->side_dist.y = (player->pos.y - d->move.y) * d->delta_dist.y;
	}
	else
	{
		d->step.y = 1;
		d->side_dist.y = (d->move.y + 1.0 - player->pos.y) * d->delta_dist.y;
	}
}

void			*build_walls(void *param)
{
	double		camera;
	t_data		*d;

	d = (t_data*)param;
	while (d->start_x < d->end_x)
	{
		camera = (d->start_x << 1) / (double)d->win_size.x - 1;
		d->ray.x = d->player->dir.x + d->player->plane.x * camera;
		d->ray.y = d->player->dir.y + d->player->plane.y * camera;
		d->move.x = (int)d->player->pos.x;
		d->move.y = (int)d->player->pos.y;
		d->delta_dist.x = fabs(1 / d->ray.x);
		d->delta_dist.y = fabs(1 / d->ray.y);
		calculate_side_dist(d, d->player);
		calculate_strip_wall(d->player, d->game, d);
		d->wall_size = (int)(d->win_size.y / d->wall_dist);
		d->half_wall_size = d->wall_size >> 1;
		draw_strip_of_wall(d->game, d->player, d);
		d->start_x++;
	}
	return (NULL);
}
