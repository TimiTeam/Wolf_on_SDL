/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   head.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 19:18:52 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 19:24:21 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HEAD_H
# define __HEAD_H

# include <fcntl.h>
# include <pthread.h>
# include <time.h>
# include <math.h>
# include "libft.h"
# include "pixel_worker.h"
# include <SDL.h>
# include "SDL_mixer.h"

# define RES_IMG "res/res.txt"
# define RES_MENU "res/menu/list.txt"
# define RES_MAPS "maps/maps.txt"
# define COUNT_MAPS 5
# define THREADS 4
# define CO_TEXT 18
# define ERROR -1
# define OK 0
# define EXIT 1
# define NEW_GAME 2
# define MENU 3
# define THE_END 4
# define CONTINUE 5
# define ROTATE 0.171

typedef	struct		s_vec
{
	double			x;
	double			y;
}					t_vec;

typedef	struct		s_distance
{
	int				start;
	int				end;
}					t_distance;

typedef	struct		s_cos_sin
{
	double			cos;
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
	Mix_Chunk		*step;
	Mix_Chunk		*door;
	int				play;
}					t_player;

typedef	struct		s_game
{
	int				**w_map;
	int				rows;
	int				*elem;
	int				floor;
	int				ceiling;
}					t_game;

typedef	struct		s_images
{
	SDL_Surface		*walls[CO_TEXT + 1];
	SDL_Surface		*surf;
}					t_images;

typedef	struct		s_sdl
{
	t_point			win_size;
	SDL_Window		*win;
	SDL_Renderer	*ren;
	Mix_Music		*music;
	Mix_Chunk		*menu_move;
	Mix_Chunk		*menu_select;
	t_images		*img;
	t_images		*menu;
	char			**maps;
	int				map;
	int				count_maps;
	int				in_game;
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

int					fill_new_t_game(t_game *g, char *path_to_map);
int					fill_new_t_player(t_player *p);
int					init_objects(t_sdl *s, t_player **p, t_game **g);
int					new_clear_objects(t_sdl *s, t_player *p, t_game *g);
t_sdl				*new_t_sdl(int s_x, int s_y);
int					**read_and_save_map(int size, char *pth, int *rows_size);
t_player			*create_player();
t_game				*create_game(char *pth_to_map);
void				create_and_fill(t_data *d, t_sdl *s, int star_x, int step);
int					valid_map(int **map, int size, int *rows_size);
int					arr_size(char **arr);
int					not_found(char *mess);
t_images			*create_and_images(char *file_list);
SDL_Surface			*load_surface(char *pth);
Mix_Chunk			*load_effects(char *path);
Mix_Music			*load_music(char *path);

void				clear_image(t_images *im);
int					close_all(t_sdl *s);
int					destroy_game(t_game *g);
int					exit_x(t_sdl *s, t_player *p, t_game *g);
int					error_message(char const *m, t_sdl *s,
		t_player *p, t_game *g);
int					map_error(char *mes);
void				free_void_map(void **map, int size);

void				find_free_place(t_game *g, t_vec *pos);
int					game_loop(t_sdl *s, t_player *p, t_game *g);
void				*build_walls(void *param);
void				draw_strip_of_wall(t_game *g, t_player *p, t_data *d);
int					get_num_texture(t_data *d, int map_tex);
int					make_actions(SDL_Keycode k, t_sdl *s,
		t_player *p, t_game *g);
int					show_menu(t_sdl *s);
void				render_manipulations(SDL_Renderer *ren, SDL_Texture *tex);

#endif
