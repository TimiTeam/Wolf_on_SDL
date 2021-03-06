/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 18:58:05 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 17:51:16 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

char			**read_maps_path(char *file_name)
{
	char		buf[256 + 1];
	int			fd;
	int			rd;

	if ((fd = open(file_name, O_RDONLY)) < 1)
	{
		ft_putstr("File not found: ");
		ft_putendl(file_name);
		return (NULL);
	}
	if ((rd = read(fd, &buf, 256)) < 1)
	{
		ft_putstr("File is broken\n");
		return (NULL);
	}
	buf[rd] = '\0';
	return (ft_strsplit(buf, '\n'));
}

int				load_data(t_sdl *s)
{
	if (!(s->img = create_and_images("res/res.txt")))
		return (ERROR);
	s->img->surf = SDL_CreateRGBSurface(0, s->win_size.x, s->win_size.y,
			32, 0, 0, 0, 255);
	if (!(s->menu = create_and_images("res/menu/list.txt")))
		return (ERROR);
	if (!(s->maps = read_maps_path("maps/maps.txt")))
		return (ERROR);
	s->music = load_music("res/8bitMelody.xm");
	s->menu_move = load_effects("res/menu.wav");
	s->menu_select = load_effects("res/select.wav");
	s->count_maps = arr_size(s->maps);
	return (OK);
}

int				init_sdl_elem(t_sdl *s)
{
	int			flag;

	flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		return (ERROR);
	s->win = SDL_CreateWindow("wolf3d", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, s->win_size.x, s->win_size.y, SDL_WINDOW_SHOWN);
	if (!s->win)
		return (ERROR);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		ft_putendl(Mix_GetError());
		return (ERROR);
	}
	s->ren = SDL_CreateRenderer(s->win, -1, flag);
	if (!s->ren)
		return (ERROR);
	if (load_data(s) == ERROR)
		return (ERROR);
	s->menu->surf = NULL;
	return (OK);
}

int				init_objects(t_sdl *s, t_player **p, t_game **g)
{
	*p = NULL;
	*g = NULL;
	if (init_sdl_elem(s) == ERROR)
		return (error_message(SDL_GetError(), s, NULL, NULL));
	if (!(*g = create_game(NULL)))
		return (error_message("Failed to create game", s, NULL, *g));
	if (!(*p = create_player()))
		return (error_message("Failed to create player", s, *p, *g));
	return (OK);
}

int				new_clear_objects(t_sdl *s, t_player *p, t_game *g)
{
	if (!s || !p || !g)
		return (error_message("Missing main objects\n", s, p, g));
	if (g->w_map)
		free_void_map((void**)g->w_map, g->rows);
	if (g->elem)
		free(g->elem);
	g->w_map = NULL;
	g->elem = NULL;
	s->map = s->map >= s->count_maps ? 0 : s->map;
	if (fill_new_t_game(g, s->maps[s->map]) == ERROR)
		return (error_message("Can't create Game", s, p, g));
	g->floor = s->map == s->count_maps - 1 ? CO_TEXT - 1 : CO_TEXT - 2;
	g->ceiling = s->map == s->count_maps - 1 ? CO_TEXT - 4 : CO_TEXT - 3;
	fill_new_t_player(p);
	find_free_place(g, &p->pos);
	s->in_game = 1;
	return (OK);
}
