/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 19:09:55 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 19:12:37 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static void		find_side(t_vec *floor_wall, t_data *d, double wall_x)
{
	if (!d->side && d->ray.x > 0)
	{
		floor_wall->x = d->move.x;
		floor_wall->y = d->move.y + wall_x;
	}
	else if (!d->side && d->ray.x < 0)
	{
		floor_wall->x = d->move.x + 1.0;
		floor_wall->y = d->move.y + wall_x;
	}
	else if (d->side && d->ray.y > 0)
	{
		floor_wall->x = d->move.x + wall_x;
		floor_wall->y = d->move.y;
	}
	else
	{
		floor_wall->x = d->move.x + wall_x;
		floor_wall->y = d->move.y + 1.0;
	}
}

static void		floor_ceiling(t_pixel_pos tr, t_data *d, double wall_x)
{
	t_vec		point;
	t_vec		curr_pix;
	double		current_dist;
	double		weight;
	Uint32		pixel;

	find_side(&point, d, wall_x);
	while (tr.dis.start < tr.dis.end)
	{
		current_dist = d->win_size.y / (2.0 * tr.dis.start - d->win_size.y);
		weight = current_dist / d->wall_dist;
		weight = fabs(weight);
		curr_pix.x = weight * point.x + (1.0 - weight) * d->player->pos.x;
		curr_pix.y = weight * point.y + (1.0 - weight) * d->player->pos.y;
		tr.src_point.x = (int)(curr_pix.x * tr.src_surf->w) % tr.src_surf->w;
		tr.src_point.y = (int)(curr_pix.y * tr.src_surf->h) % tr.src_surf->h;
		if (tr.src_point.y < 0)
			tr.src_point.y *= -1;
		if (tr.src_point.x < 0)
			tr.src_point.x *= -1;
		tr.dst_point.y = tr.dis.start;
		pixel = get_pixel(tr.src_surf, tr.src_point);
		put_pixel(tr.dst_surf, tr.dst_point, pixel);
		tr.dis.start++;
	}
}

static void		draws(t_distance dis, t_pixel_pos tr, t_data *d, double w_x)
{
	Uint32		pixel;
	int			dt;

	tr.src_surf = d->img->walls[d->num_tex];
	tr.src_point.x = (int)(w_x * (double)tr.src_surf->w);
	if ((d->side == 0 && d->ray.x > 0) || (d->side == 1 && d->ray.y < 0))
		tr.src_point.x = tr.src_surf->w - tr.src_point.x - 1;
	while (dis.start < dis.end)
	{
		dt = dis.start * 256 - d->win_size.y * 128 + d->wall_size * 128;
		tr.dst_point.y = dis.start;
		tr.src_point.y = ((dt * tr.src_surf->h) / d->wall_size) / 256;
		pixel = get_pixel(d->img->walls[d->num_tex], tr.src_point);
		put_pixel(d->img->surf, tr.dst_point, pixel);
		dis.start++;
	}
	if (dis.end < d->win_size.y)
	{
		tr.dis.start = dis.end;
		tr.dis.end = d->win_size.y;
		tr.src_surf = d->img->walls[d->game->floor];
		floor_ceiling(tr, d, w_x);
	}
}

void			draw_strip_of_wall(t_game *g, t_player *p, t_data *d)
{
	double		wall_x;
	t_distance	dist;
	t_pixel_pos	tr;

	dist.start = -d->half_wall_size + d->half_win_y;
	dist.start = dist.start < 0 ? 0 : dist.start;
	dist.end = d->half_wall_size + d->half_win_y;
	dist.end = dist.end >= d->win_size.y ? d->win_size.y : dist.end;
	d->num_tex = get_num_texture(d, g->w_map[d->move.y][d->move.x] - 1);
	tr.dst_surf = d->img->surf;
	tr.dst_point.x = d->start_x;
	tr.dis.start = 0;
	if (!d->side)
		wall_x = p->pos.y + d->wall_dist * d->ray.y;
	else
		wall_x = p->pos.x + d->wall_dist * d->ray.x;
	wall_x -= floor(wall_x);
	if (dist.start > 0)
	{
		tr.dis.end = dist.start;
		tr.src_surf = d->img->walls[d->game->ceiling];
		floor_ceiling(tr, d, wall_x);
	}
	draws(dist, tr, d, wall_x);
}
