#include "head.h"

void 		find_free_place(t_game *s, t_vec *pos)
{
	int 	i;
	int 	j;

	i = 0;
	while (i < s->rows)
	{
		j = 0;
		while (j < s->elem[i])
		{
			if (s->w_map[i][j] == 0 )
			{
				pos->x = i + 0.2;
				pos->y = j + 0.2;
				return ;
			}
			j++;
		}
		i++;
	}
}

void 		show_map(int **map, int size, int *rows_size)
{
	int 	i;
	int 	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < rows_size[i])
		{
			ft_putnbr(map[i][j]);
			write(1, " ", 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

int				copy_surf(t_pixel_buf pix_buf)
{
	SDL_Rect	d;
	SDL_Rect	s;

	d.x = pix_buf.dst_point.x;
	d.y = pix_buf.dst_point.y;
	d.w = 1;
	d.h = 1;
	s.x = pix_buf.src_point.x;
	s.y = pix_buf.src_point.y;
	s.w = 1;
	s.h = 1;
	SDL_BlitSurface(pix_buf.src_surf , &s, pix_buf.dst_surf, &d);
	return (0);
}

void 			floor_ceiling(t_pixel_buf tr, t_sdl *s, t_game *g, t_player *p, double wall_x)
{
	t_vec		floor_wall;
	t_vec		curr_pint;
	double		currentDist;
	double 		weight;

	if (!g->side && p->ray.x > 0)
	{
		floor_wall.x = g->m.x;
		floor_wall.y = g->m.y + wall_x;
	}
	else if (!g->side && p->ray.x < 0)
	{
		floor_wall.x = g->m.x + 1.0;
		floor_wall.y = g->m.y + wall_x;
	}
	else if (g->side && p->ray.y > 0)
	{
		floor_wall.x = g->m.x + wall_x;
		floor_wall.y = g->m.y;
	}
	else
	{
		floor_wall.x = g->m.x + wall_x;
		floor_wall.y = g->m.y + 1.0;
	}
	while (tr.from_y <= tr.to_y)
	{
		currentDist = s->win_size.y / (2.0 * tr.from_y - s->win_size.y);
		weight = currentDist / g->wall_dist;
		weight = fabs(weight);
		curr_pint.x = weight * floor_wall.x + (1.0 - weight) * p->pos.x;
		curr_pint.y = weight * floor_wall.y + (1.0 - weight) * p->pos.y;
		tr.src_point.x = (int)(curr_pint.x * tr.src_surf->w) % tr.src_surf->w;
		tr.src_point.y = (int)(curr_pint.y * tr.src_surf->h) % tr.src_surf->h;
		if (tr.src_point.y < 0)
			tr.src_point.y *= -1;
		if (tr.src_point.x < 0)
			tr.src_point.x *= -1;
		tr.dst_point.y = tr.from_y;
		copy_surf(tr);
		tr.from_y++;
	}
}

void 			draw_strip_of_wall(int x, t_sdl *s, t_game *g, t_player *p)
{
	int			num_tex;
	double		wall_x;
	int			d;
	int			start_y;
	int			end_y;
	t_pixel_buf	transport; //

	start_y = -g->half_wall_size + g->half_win_y;
	start_y = start_y < 0 ? 0 : start_y;
	end_y  = g->half_wall_size + g->half_win_y;
	end_y  = end_y  >= s->win_size.y ? s->win_size.y - 1 : end_y;
	num_tex = g->w_map[g->m.y][g->m.x] - 1;

	transport.dst_surf = s->surf;
	transport.dst_point.x = x;
	transport.from_y = 0;
	if (!g->side)
		wall_x = p->pos.y + g->wall_dist * p->ray.y;
	else
		wall_x = p->pos.x + g->wall_dist * p->ray.x;
	wall_x -= floor(wall_x);
	if (start_y > 0)
	{
		transport.to_y = start_y;
		transport.src_surf = s->walls[COUNT_TEXT - 2];
		floor_ceiling(transport, s, g, p, wall_x);
	}
	transport.src_surf = s->walls[num_tex];
	transport.src_point.x = (int)(wall_x * (double)TEXTURE_W);
	if ((g->side == 0 && p->ray.x > 0) || (g->side == 1 && p->ray.y < 0))
		transport.src_point.x = TEXTURE_W - transport.src_point.x - 1;
	while (start_y < end_y)
	{
		d = start_y * 256 - s->win_size.y * 128 + g->wall_size * 128;
		transport.dst_point.y = start_y;
		transport.src_point.y = ((d * transport.src_surf->h) / g->wall_size) / 256;
		copy_surf(transport);
		start_y++;
	}
	if (start_y < s->win_size.y)
	{
		transport.from_y = start_y;
		transport.to_y = s->win_size.y;
		transport.src_surf = s->walls[COUNT_TEXT - 1];
		floor_ceiling(transport, s, g, p, wall_x);
	}
}

void			calculate_strip_wall(t_player *p, t_game *game)
{
	while (1)
	{
		if (game->side_dist.x < game->side_dist.y)
		{
			game->side_dist.x += game->delta_dist.x;
			game->m.x += game->step.x;
			game->side = 0;
		}
		else
		{
			game->side_dist.y += game->delta_dist.y;
			game->m.y += game->step.y;
			game->side = 1;
		}
		if (game->w_map[game->m.y][game->m.x] > 0)
			break ;
	}
	if (game->side)
		game->wall_dist = (game->m.y - p->pos.y + (double)(1 - game->step.y) / 2) / p->ray.y;
	else
		game->wall_dist = (game->m.x - p->pos.x + (double)(1 - game->step.x) / 2) / p->ray.x;
	game->wall_dist = game->wall_dist <= 0 ? 1.0 : game->wall_dist;
}

void			calculate_side_dist(t_player *player, t_game *game)
{
	if (player->ray.x < 0)
	{
		game->step.x = -1;
		game->side_dist.x = (player->pos.x - game->m.x) * game->delta_dist.x;
	}
	else
	{
		game->step.x = 1;
		game->side_dist.x = (game->m.x + 1.0 - player->pos.x) * game->delta_dist.x;
	}
	if (player->ray.y < 0)
	{
		game->step.y = -1;
		game->side_dist.y = (player->pos.y - game->m.y) * game->delta_dist.y;
	}
	else
	{
		game->step.y = 1;
		game->side_dist.y = (game->m.y + 1.0 - player->pos.y) * game->delta_dist.y;
	}
}

void 			build_walls(t_sdl *s)
{
	double		camera;
	int			x;

	x = 0;
	while (x < s->win_size.x)
	{
		camera = x * 2 / (double)s->win_size.x - 1;
		s->player->ray.x = s->player->dir.x + s->player->plane.x * camera;
		s->player->ray.y = s->player->dir.y + s->player->plane.y * camera;
		s->game->m.x = (int)s->player->pos.x;
		s->game->m.y = (int)s->player->pos.y;
		s->game->delta_dist.x = fabs(1 / s->player->ray.x);
		s->game->delta_dist.y = fabs(1 / s->player->ray.y);
		calculate_side_dist(s->player, s->game);
		calculate_strip_wall(s->player, s->game);
		s->game->wall_size = (int)(s->win_size.y / s->game->wall_dist);
		s->game->half_wall_size = s->game->wall_size / 2;
		draw_strip_of_wall(x, s, s->game, s->player);
		x++;
	}
}

void 			rotate_player(t_player *p, t_cos_sin cos_sin)
{
	double 		oldDirX;
	double 		oldPlaneX;

	oldDirX = p->dir.x;
	oldPlaneX = p->plane.x;
	p->dir.x = p->dir.x * cos_sin.cos - p->dir.y * cos_sin.sin;
	p->dir.y = oldDirX * cos_sin.sin + p->dir.y * cos_sin.cos;
	p->plane.x = p->plane.x * cos_sin.cos - p->plane.y * cos_sin.sin;
	p->plane.y = oldPlaneX * cos_sin.sin + p->plane.y * cos_sin.cos;
}

void 			print_info(t_player	*p, t_sdl *s)
{
	printf("\t\t** INFO **\nposition x = %f, y = %f;\ndirection x = %f, y = %f;"    // FORBIDEN FUNCTION
	"\nray direction x = %f, y = %f;\nplane x = %f, y = %f\n\n", p->pos.x, p->pos.y, 
	p->dir.x, p->dir.y, p->ray.x, p->ray.y, p->plane.x, p->plane.y);
	printf("fps  %f\n", (double)(s->end - s->start) / CLOCKS_PER_SEC);
}

void			make_actions(SDL_Keycode k, t_sdl *s)
{
	t_game		*g;
	t_player	*p;

	g = s->game;
	p = s->player;
	switch (k)
	{
		case SDLK_UP:
			if (g->w_map[(int)(p->pos.y + p->dir.y * SPEED)][(int)p->pos.x] == 0)
				p->pos.y += p->dir.y * SPEED;
			if (g->w_map[(int)p->pos.y][(int)(p->pos.x + p->dir.x * SPEED)] == 0)
				p->pos.x += p->dir.x * SPEED;
			break ;
		case SDLK_DOWN:
			if (g->w_map[(int)(p->pos.y - p->dir.y * SPEED)][(int)p->pos.x] == 0)
				p->pos.y -= p->dir.y * SPEED;
			if (g->w_map[(int)p->pos.y][(int)(p->pos.x - p->dir.x * SPEED)] == 0)
				p->pos.x -= p->dir.x * SPEED;
			break ;
		case SDLK_LEFT:
			rotate_player(p, p->plus);
			break ;
		case SDLK_RIGHT:
			rotate_player(p, p->minus);
			break ;
		case SDLK_i :
			print_info(p,s);
			break ;
		case SDLK_c :
			system("clear");
			break ;
		default:
			break;
	}
}

int				game_loop(t_sdl *s, t_player *p, t_game *g)
{
	SDL_Event	e;
	SDL_Texture	*tex;

	tex = NULL;
	while(1)
	{
		s->start = clock();
		s->surf = SDL_CreateRGBSurface(0, s->win_size.x, s->win_size.y, 32, 0, 0 ,0, 255);
		build_walls(s);
		if (tex)
			SDL_DestroyTexture(tex);
		tex = SDL_CreateTextureFromSurface(s->ren, s->surf);
		SDL_RenderCopy(s->ren, tex, NULL, NULL);
		SDL_RenderPresent(s->ren);
		SDL_FreeSurface(s->surf);
		s->end = clock();
	//	printf("fps  %f\n", (double)(s->end - s->start) / CLOCKS_PER_SEC);
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				return (0);
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
					return (0);
				if (e.key.keysym.sym == SDLK_q)
					return (MENU);
				else
					make_actions(e.key.keysym.sym, s);
			}
		}
	}
	return (1);
}

int				start_game(t_sdl *sdl, t_game *game)
{
	int			ret;
	t_player 	*player;
	
	ret = 0;
	if (!(player = create_player()))
		return (error_message("Create player"));
	find_free_place(game, &player->pos);
	sdl->player = player;
	ret = game_loop(sdl, player, game);
	if (ret == MENU)
		ret = NEW_GAME;
	free (player);
	return (ret);
}
