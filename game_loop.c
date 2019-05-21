#include "head.h"

static void 		find_side(t_vec *floor_wall, t_data *d, double wall_x)
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

static void 		floor_ceiling(t_pixel_pos tr, t_data *d, double wall_x)
{
	t_vec			point;
	t_vec			curr_pix;
	double			currentDist;
	double 			weight;
	Uint32			pixel;

	find_side(&point, d, wall_x);
	while (tr.dis.start < tr.dis.end)
	{
		currentDist = d->win_size.y / (2.0 * tr.dis.start - d->win_size.y);
		weight = currentDist / d->wall_dist;
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

static int			get_num_texture(t_data *d)
{
	if (d->side == 0 && d->ray.x < 0)
		return 0;
	else if (d->side == 0 && d->ray.x > 0)
		return (1);
	else if (d->side == 1 && d->ray.y > 0)
		return (2);
	else if (d->side == 1 && d->ray.y < 0)
		return (3);
	else
		return (CO_TEXT - 1);
}

static void 		draw_strip_of_wall(t_game *g, t_player *p, t_data *d)
{
	Uint32			pixel;
	double			wall_x;
	int				dt;
	t_distance		dist;
	t_pixel_pos		tr;

	dist.start = -d->half_wall_size + d->half_win_y;
	dist.start = dist.start < 0 ? 0 : dist.start;
	dist.end = d->half_wall_size + d->half_win_y;
	dist.end  = dist.end  >= d->win_size.y ? d->win_size.y : dist.end;

	if (!(d->num_tex = g->w_map[d->move.y][d->move.x] - 1))
		d->num_tex = get_num_texture(d);
	if (d->num_tex >= CO_TEXT)
		d->num_tex = CO_TEXT - 1;
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
	tr.src_surf = d->img->walls[d->num_tex];
	tr.src_point.x = (int)(wall_x * (double)tr.src_surf->w);
	if ((d->side == 0 && d->ray.x > 0) || (d->side == 1 && d->ray.y < 0))
		tr.src_point.x = tr.src_surf->w - tr.src_point.x - 1;
	while (dist.start < dist.end)
	{
		dt = dist.start * 256 - d->win_size.y * 128 + d->wall_size * 128;
		tr.dst_point.y = dist.start;
		tr.src_point.y = ((dt * tr.src_surf->h) / d->wall_size) / 256;
		pixel = get_pixel(d->img->walls[d->num_tex], tr.src_point);
		put_pixel(d->img->surf, tr.dst_point, pixel);
		dist.start++;
	}
	if (dist.end < d->win_size.y)
	{
		tr.dis.start = dist.end;
		tr.dis.end = d->win_size.y;
		tr.src_surf = d->img->walls[d->game->floor];
		floor_ceiling(tr, d, wall_x);
	}
}

static void			calculate_strip_wall(t_player *p, t_game *game, t_data *d)
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
		if (game->w_map[d->move.y][d->move.x] > 0)
			break ;
	}
	if (d->side)
		d->wall_dist = (d->move.y - p->pos.y + (double)(1 - d->step.y) / 2) / d->ray.y;
	else
		d->wall_dist = (d->move.x - p->pos.x + (double)(1 - d->step.x) / 2) / d->ray.x;
	d->wall_dist = d->wall_dist <= 0 ? 1.0 : d->wall_dist;
}

static void			calculate_side_dist(t_data *d, t_player *player)
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

static void 			*build_walls(void *param)
{
	double		camera;
	t_data 		*d;

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

int				run_raycasting_threads(t_sdl *s, t_player *p, t_game *g)
{
	t_data		*data[THREADS];
	pthread_t 	thread[THREADS];
	int			i;
	int			step;

	step = s->win_size.x / THREADS - 1;
	i = 0;
	while (i < THREADS)
	{
		if (!(data[i] = create_and_fill(s, i * step, step)))
			return (ERROR);
		data[i]->player = p;
		data[i]->game = g;
		pthread_create(&thread[i], NULL, build_walls, data[i]);
		i++;
	}
	i = 0;
	while (i < THREADS)
	{
		pthread_join(thread[i], NULL);
		free(data[i]);
		i++;
	}
	return (OK);
}

int				game_loop(t_sdl *s, t_player *p, t_game *g)
{
	SDL_Event	e;
	SDL_Texture	*tex;
	int			stop;

	stop = 0;
	while(!stop)
	{
		s->start = clock();
		run_raycasting_threads(s, p, g);
		tex = SDL_CreateTextureFromSurface(s->ren, s->img->surf);
		SDL_RenderCopy(s->ren, tex, NULL, NULL);
		SDL_RenderPresent(s->ren);
		SDL_DestroyTexture(tex);
		s->end = clock();
		p->speed = (double)(s->end - s->start) / CLOCKS_PER_SEC * 4.5;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				stop = EXIT;
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
					stop = EXIT;
				if (e.key.keysym.sym == SDLK_m)
					stop = MENU;
				else
					if(make_actions(e.key.keysym.sym, s, p, g) == NEW_GAME)
						stop = NEW_GAME;
			}
		}
	}
	return (stop);
}
