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

void 			draw_strip_of_wall(int x, t_sdl *s, t_game *g, t_player *p)
{
	int			num_tex;
	double		wall_x;
	int			tex_x;
	int			tex_y;
	int			y;
	int			d;
	SDL_Surface *sur;
	int			start;
	int			end;
	
	start = -g->half_wall_size + g->half_win_y;
	if (start < 0)
		start = 0;
	end = g->half_wall_size + g->half_win_y;
	if (end >= s->win_size.y)
		end = s->win_size.y - 1;

	num_tex = g->w_map[g->m.y][g->m.x] - 1;
//	num_tex = num_tex >= COUNT_TEXT ? COUNT_TEXT - 2 : num_tex;

	if (!g->side)
		wall_x = p->pos.y + g->wall_dist * p->ray.y;
	else
		wall_x = p->pos.x + g->wall_dist * p->ray.x;
	wall_x -= floor(wall_x);

	tex_x = (int)(wall_x * (double)TEXTURE_W);
	if (g->side == 0 && p->ray.x > 0)
		tex_x = TEXTURE_W - tex_x - 1;
	if (g->side == 1 && p->ray.y < 0)
		tex_x = TEXTURE_W - tex_x - 1;

	sur = s->walls[num_tex];
	y = start;
	while (y < end)
	{
		d = y * 256 - s->win_size.y * 128 + g->wall_size * 128;
		tex_y = ((d * sur->h) / g->wall_size) / 256;
		t_point to;
		t_point	from;
		to.x = x;
		to.y = y;
		from.x = tex_x;
		from.y = tex_y;
		copy_surf(s->surf, sur, to, from);
		y++;
	}
}

void			calculate_strip_wall(t_player *p, t_game *game)
{
	double		dist_to_wall;

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
	if (game->wall_dist <= 0)
		game->wall_dist = 1.0;
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
	int			strip_size;

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

void 			rotate_player(t_player *p, double rotate)
{
	double 		oldDirX;
	double 		oldPlaneX;
	double 		cos_rotate;
	double 		sin_rotate;

	oldDirX = p->dir.x;
	oldPlaneX = p->plane.x;
	cos_rotate = cos(rotate);
	sin_rotate = sin(rotate);
	p->dir.x = p->dir.x * cos_rotate - p->dir.y * sin_rotate;
	p->dir.y = oldDirX * sin_rotate + p->dir.y * cos_rotate;
	p->plane.x = p->plane.x * cos_rotate - p->plane.y * sin_rotate;
	p->plane.y = oldPlaneX * sin_rotate + p->plane.y * cos_rotate;
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
			rotate_player(p, ROTATE);
			break ;
		case SDLK_RIGHT:
			rotate_player(p, -ROTATE);
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
		SDL_SetRenderDrawColor (s->ren, 0, 0, 0, 255);
		SDL_RenderClear(s->ren);
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
