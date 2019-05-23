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
	int			i;

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
	if (line)
		ft_strdel(&line);
	return (s);
}

int			fill_new_t_game(t_game *g, char *path_to_map)
{
	int		fd;
	if (!g)
	{
		ft_putendl("Need create t_game struct\n");
		return (ERROR);
	}
	if (((fd = open(path_to_map, O_RDONLY)) < 1 ||
			(g->rows = get_map_size(fd)) == ERROR))
	{
		ft_putstr("File not found: ");
		ft_putendl(path_to_map);
		return (ERROR);
	}
	close(fd);
	g->elem = (int*)ft_memalloc(sizeof(int) * g->rows);
	if (!(g->w_map = read_and_save_map(g->rows, path_to_map, g->elem)))
	{
		ft_putstr("Wrong file or file is broken\n");
		return (ERROR);
	}
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
			destroy_game (g);
	}
	return (g);
}


int			fill_new_t_player(t_player *p)
{
	if (!p)
		return (ERROR);
	p->dir.x = 0.305253;
	p->dir.y = 0.952271;
	p->plane.x = 0.818953;
	p->plane.y = -0.262517;
	p->plus.cos = cos(ROTATE);
	p->plus.sin = sin(ROTATE);
	p->minus.cos = cos(-ROTATE);
	p->minus.sin = sin(-ROTATE);
	p->speed = 0.091;
	return (OK);
}

t_player		*create_player(void)
{
	t_player	*p;

	if (!(p = (t_player*)ft_memalloc(sizeof(t_player))))
		return (NULL);
	if (fill_new_t_player(p) == ERROR)
		return (NULL);
	p->step = load_effects("res/step.wav");
	p->door = load_effects("res/closing door.wav");
	p->play = 1;
	return (p);
}
