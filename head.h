#ifndef __HEAD_H
#define __HEAD_H

#include <fcntl.h>
#include <math.h>
#include "libft.h"
#include "SDL.h"

#define	COUNT_TEXT 9
#define TEXTURE_W 64
#define TEXTURE_H 64
#define ERROR -1
#define OK 0
#define NEW_GAME 2
#define MENU 3
#define SPEED 0.2
#define ROTATE 0.2

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
	
typedef struct		s_player
{
	t_vec			pos;
	t_vec			dir;
	t_vec			plane;
	t_vec			ray;
}					t_player;

typedef	struct		s_game
{
	t_point			m;
	t_point			step;
	int				side;
	double			wall_dist;
	int				wall_size;
	int				half_wall_size;
	int				half_win_y;
	t_vec			side_dist;
	t_vec			delta_dist;
	int				**w_map;
	int				rows;
	int				*elem;
}					t_game;

typedef	struct		s_sdl
{
	t_point			win_size;
	SDL_Window		*win;
	SDL_Renderer	*ren;
	SDL_Surface		*surf;
	SDL_Surface		*walls[COUNT_TEXT];
	t_player		*player;
	t_game			*game;
	char			*path_map;
}					t_sdl;

int					init_sdl_elem(t_sdl *s);
t_sdl				*new_t_sdl(int s_x, int s_y);
int 				**read_and_save_map(int size, char *pth, int *rows_size);
t_player			*create_player();
t_game				*create_game(char *pth_to_map);

int					close_all(t_sdl *s);
int					clear_fields_t_game(t_game *g);
int 				destroy_game(t_game *g);
int					error_exit(char	const *mess, t_sdl *s, t_game *g);
int 				error_message(char const *mes);
void 				free_void_map(void **map, int size);

int					start_game(t_sdl *s, t_game *g);

#endif
