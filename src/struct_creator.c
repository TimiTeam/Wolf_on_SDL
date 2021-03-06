/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_creator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:06:08 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/14 19:08:58 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

t_sdl			*new_t_sdl(int s_x, int s_y)
{
	t_sdl		*s;

	if (!(s = (t_sdl*)ft_memalloc(sizeof(t_sdl))))
		return (NULL);
	s->win_size.x = s_x;
	s->win_size.y = s_y;
	s->win = NULL;
	s->ren = NULL;
	s->img = NULL;
	s->menu = NULL;
	s->music = NULL;
	s->map = 0;
	s->maps = NULL;
	s->in_game = 0;
	return (s);
}

static int		check_line(char *l)
{
	int			i;

	i = 0;
	while (l[i])
	{
		if ((l[i] < '0' || l[i] > '9') && l[i] != ' ' && l[i] != ',')
			return (ERROR);
		i++;
	}
	return (0);
}

int				get_map_size(int fd)
{
	char		*line;
	int			s;

	s = 0;
	line = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		s++;
		if (check_line(line) == ERROR)
		{
			ft_strdel(&line);
			return (ERROR);
		}
		ft_strdel(&line);
	}
	if (s > 0)
		ft_strdel(&line);
	else
	{
		ft_strdel(&line);
		return (map_error("Empty map"));
	}
	return (s);
}

int				fill_new_t_game(t_game *g, char *path_to_map)
{
	int			fd;

	if (!g)
		return (ERROR);
	if (((fd = open(path_to_map, O_RDONLY)) < 1 ||
			(g->rows = get_map_size(fd)) == ERROR))
		return (not_found(path_to_map));
	close(fd);
	g->elem = (int*)ft_memalloc(sizeof(int) * g->rows);
	if ((g->w_map = read_and_save_map(g->rows, path_to_map, g->elem)))
	{
		if (valid_map(g->w_map, g->rows, g->elem) == ERROR)
			return (map_error("Map error"));
	}
	else
		return (map_error("Wrong file or file is broken"));
	return (OK);
}

t_game			*create_game(char *map_name)
{
	t_game		*g;

	if (!(g = (t_game*)ft_memalloc(sizeof(t_game))))
		return (NULL);
	g->rows = 0;
	g->w_map = NULL;
	g->elem = NULL;
	if (map_name)
	{
		if (fill_new_t_game(g, map_name) == ERROR)
			destroy_game(g);
	}
	return (g);
}
