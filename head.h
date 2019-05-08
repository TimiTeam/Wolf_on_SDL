#ifndef __HEAD_H
#define __HEAD_H

#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include "libft.h"
#include "pixel_worker.h"
#include <SDL.h>

#define THREADS 4
#define	COUNT_TEXT 12
#define TEXTURE_W 64
#define TEXTURE_H 64
#define ERROR -1
#define OK 0
#define NEW_GAME 2
#define MENU 3
#define ROTATE 0.13

typedef	struct		s_vec
{
	double			x;
	double			y;
}					t_vec;

typedef	struct		s_point
{
	int				x;
	int				y;
}					t_point;

typedef	struct 		s_distance
{
	int				start;
	int 			end;
}					t_distance;

typedef	struct		s_cos_sin
{
	double 			cos;
	double			sin;
}					t_cos_sin;

typedef	struct		s_pixel_pos
{
	SDL_Surface		*src_surf;
	t_point			src_point;
	SDL_Surface		*dst_surf;
	t_point			dst_point;
	t_distance		dis;
}					t_pixel_pos;

typedef struct		s_player
{
	t_vec			pos;
	t_vec			dir;
	t_vec			plane;
	t_cos_sin		plus;
	t_cos_sin		minus;
	double			speed;
}					t_player;

typedef	struct		s_game
{
	int				**w_map;
	int				rows;
	int				*elem;
}					t_game;

typedef	struct		s_images
{
	SDL_Surface		*walls[COUNT_TEXT];
	SDL_Surface		*surf;
}					t_images;


typedef	struct		s_sdl
{
	t_point			win_size;
	SDL_Window		*win;
	SDL_Renderer	*ren;
	t_images		*img;
	char			*path_map;
	time_t			start;
	time_t			end;
}					t_sdl;

typedef struct		s_data
{
	t_player		*player;
	t_game			*game;
	t_images		*img;
	t_point			win_size;
	t_vec			ray;
	t_point			move;
	t_point			step;
	int				num_tex;
	int				side;
	double			wall_dist;
	int				wall_size;
	int				half_wall_size;
	int				half_win_y;
	t_vec			side_dist;
	t_vec			delta_dist;
	int				start_x;
	int				end_x;
}					t_data;

int					init_sdl_elem(t_sdl *s);
int					init_objects(t_sdl *s, t_player **p, t_game **g);
t_sdl				*new_t_sdl(int s_x, int s_y);
int 				**read_and_save_map(int size, char *pth, int *rows_size);
t_player			*create_player();
t_game				*create_game(char *pth_to_map);
t_data				*create_and_fill(t_sdl *s, int star_x, int step);
t_images			*create_and_images(char *file_list, SDL_Renderer *ren);

int					close_all(t_sdl *s);
int 				destroy_game(t_game *g);
int					error_exit(char const *mess, t_sdl *s, t_player *p, t_game *g);
int 				error_message(char const *mes);
void 				free_void_map(void **map, int size);

void 				find_free_place(t_game *g, t_vec *pos);
int					start_game(t_sdl *sdl, t_player *player, t_game *game);
void				make_actions(SDL_Keycode k, t_sdl *s, t_player *p, t_game *g);

#endif
