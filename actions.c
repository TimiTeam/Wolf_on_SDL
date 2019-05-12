#include "head.h"

static void 			rotate_player(t_player *p, t_cos_sin cos_sin)
{
	double 				old_dirX;
	double 				old_planeX;

	old_dirX = p->dir.x;
	old_planeX = p->plane.x;
	p->dir.x = p->dir.x * cos_sin.cos - p->dir.y * cos_sin.sin;
	p->dir.y = old_dirX * cos_sin.sin + p->dir.y * cos_sin.cos;
	p->plane.x = p->plane.x * cos_sin.cos - p->plane.y * cos_sin.sin;
	p->plane.y = old_planeX * cos_sin.sin + p->plane.y * cos_sin.cos;
}

void					make_actions(SDL_Keycode k, t_sdl *s, t_player *p, t_game *g)
{
	switch (k)
	{
		case SDLK_UP:
			if (g->w_map[(int)(p->pos.y + p->dir.y * p->speed)][(int)p->pos.x] == 0)
				p->pos.y += p->dir.y * p->speed;
			if (g->w_map[(int)p->pos.y][(int)(p->pos.x + p->dir.x * p->speed)] == 0)
				p->pos.x += p->dir.x * p->speed;
			break ;
		case SDLK_DOWN:
			if (g->w_map[(int)(p->pos.y - p->dir.y * p->speed)][(int)p->pos.x] == 0)
				p->pos.y -= p->dir.y * p->speed;
			if (g->w_map[(int)p->pos.y][(int)(p->pos.x - p->dir.x * p->speed)] == 0)
				p->pos.x -= p->dir.x * p->speed;
			break ;
		case SDLK_LEFT:
			rotate_player(p, p->plus);
			break ;
		case SDLK_RIGHT:
			rotate_player(p, p->minus);
			break ;
		case SDLK_i:
			printf("\nplayer dir.x = %f, dir.y = %f\nplane.x = %f, plane.y = %f;\n", p->dir.x, p->dir.y, p->plane.x, p->plane.y);
			break ;
		default:
			break;
	}
}