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

int				copy_surf(SDL_Surface *dst, SDL_Surface *src, t_point to, t_point from)
{
	SDL_Rect	d;
	SDL_Rect	s;

	d.x = to.x;
	d.y = to.y;
	d.w = 1;
	d.h = 1;
	s.x = from.x;
	s.y = from.y;
	s.w = 1;
	s.h = 1;
	SDL_BlitSurface(src, &s, dst, &d);
	return (0);
}

void 			print_floor(t_point point, t_sdl *s, t_game *g, t_player *p, double	wall_x)
{
	t_vec		floor_wall;
	t_vec		current_floor;
	t_point		floor_tex;
	double 		distWall;
	double		distPlayer;
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
	distWall = g->wall_dist;
	while (point.y <= s->win_size.y)
	{
		currentDist = s->win_size.y / (2.0 * point.y - s->win_size.y);

		weight = (currentDist - 0.0) / (distWall - 0.0);

		current_floor.x = weight * floor_wall.x + (1.0 - weight) * p->pos.x;
		current_floor.y = weight * floor_wall.y + (1.0 - weight) * p->pos.y;

		floor_tex.x = (int)(current_floor.x * s->walls[COUNT_TEXT - 1]->w) % s->walls[COUNT_TEXT - 1]->w;
		floor_tex.y = (int)(current_floor.y * s->walls[COUNT_TEXT - 1]->h) % s->walls[COUNT_TEXT - 1]->h;
		copy_surf(s->surf, s->walls[COUNT_TEXT - 1], point, floor_tex);
		point.y++;
	}
}

void 			draw_strip_of_wall(int x, t_sdl *s, t_game *g, t_player *p)
{
	int			num_tex;
	double		wall_x;
	t_point		tex;
	t_point 	to;
	int			y;
	int			d;
	int			start;
	int			end;
	
	start = -g->half_wall_size + g->half_win_y;
	start = start < 0 ? 0 : start;
	end = g->half_wall_size + g->half_win_y;
	end = end >= s->win_size.y ? s->win_size.y - 1 : end;
	num_tex = g->w_map[g->m.y][g->m.x] - 1;
	if (!g->side)
		wall_x = p->pos.y + g->wall_dist * p->ray.y;
	else
		wall_x = p->pos.x + g->wall_dist * p->ray.x;
	wall_x -= floor(wall_x);
	tex.x = (int)(wall_x * (double)TEXTURE_W);
	if ((g->side == 0 && p->ray.x > 0) || (g->side == 1 && p->ray.y < 0))
		tex.x = TEXTURE_W - tex.x - 1;
	y = start;
	to.x = x;
	while (y < end)
	{
		d = y * 256 - s->win_size.y * 128 + g->wall_size * 128;
		tex.y = ((d * s->walls[num_tex]->h) / g->wall_size) / 256;
		to.y = y;
		copy_surf(s->surf, s->walls[num_tex], to, tex);
		y++;
	}
	if (y < s->win_size.y)
		print_floor(to, s, g, p, wall_x);
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

void 			build_walls(t_sdl *sdl, t_player *player, t_game *game)
{
	double		camera;
	int			x;

	x = 0;
	while (x < sdl->win_size.x)
	{
		camera = x * 2 / (double)sdl->win_size.x - 1;
		player->ray.x = player->dir.x + player->plane.x * camera;
		player->ray.y = player->dir.y + player->plane.y * camera;
		game->m.x = (int)player->pos.x;
		game->m.y = (int)player->pos.y;
		game->delta_dist.x = fabs(1 / player->ray.x);
		game->delta_dist.y = fabs(1 / player->ray.y);
		calculate_side_dist(player, game);
		calculate_strip_wall(player, game);
		game->wall_size = (int)(sdl->win_size.y / game->wall_dist);
		game->half_wall_size = game->wall_size / 2;
		draw_strip_of_wall(x, sdl, game, player);
		x++;
	}
}

void 			rotate_player(t_player *p, t_vec cos_sin)
{
	double 		oldDirX;
	double 		oldPlaneX;

	oldDirX = p->dir.x;
	oldPlaneX = p->plane.x;
	p->dir.x = p->dir.x * cos_sin.x - p->dir.y * cos_sin.y;
	p->dir.y = oldDirX * cos_sin.y + p->dir.y * cos_sin.x;
	p->plane.x = p->plane.x * cos_sin.x - p->plane.y * cos_sin.y;
	p->plane.y = oldPlaneX * cos_sin.y + p->plane.y * cos_sin.x;
}

void 			print_info(t_player *p)
{
	printf("\t\t** INFO **\nposition x = %f, y = %f;\ndirection x = %f, y = %f;"    // FORBIDEN FUNCTION
	"\nray direction x = %f, y = %f;\nplane x = %f, y = %f\n\n", p->pos.x, p->pos.y, 
	p->dir.x, p->dir.y, p->ray.x, p->ray.y, p->plane.x, p->plane.y);
}

void			make_actions(SDL_Keycode k, t_player *p, t_game *g)
{
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
			rotate_player(p, p->pls_cos_sin);
			break ;
		case SDLK_RIGHT:
			rotate_player(p, p->min_cos_sin);
			break ;
		case SDLK_i :
			print_info(p);
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
		s->surf = SDL_CreateRGBSurface(0, s->win_size.x, s->win_size.y, 32, 0, 0 ,0, 255);
		build_walls(s, p, g);
		if (tex)
			SDL_DestroyTexture(tex);
		tex = SDL_CreateTextureFromSurface(s->ren, s->surf);
		SDL_RenderCopy(s->ren, tex, NULL, NULL);
		SDL_RenderPresent(s->ren);
		SDL_FreeSurface(s->surf);
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
					make_actions(e.key.keysym.sym, p, g);
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
	ret = game_loop(sdl, player, game);
	if (ret == MENU)
		ret = NEW_GAME;
	free (player);
	return (ret);
}
