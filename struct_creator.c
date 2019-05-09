#include "head.h"

t_sdl			*new_t_sdl(int s_x, int s_y)
{
	t_sdl		*s;
	int 		i;

	if(!(s = (t_sdl*)malloc(sizeof(t_sdl))))
		return (NULL);
	s->win_size.x = s_x;
	s->win_size.y = s_y;
	s->win = NULL;
	s->ren = NULL;
	s->img = NULL;
	s->path_map = NULL;
	i = 0;
	return (s);
}

static int			check_line(char *l)
{
	int				i;

	i = 0;
	while (l[i])
	{
		if ((l[i] < '0' || l[i] > '9') && l[i] != ' ' && l[i] != ',')
			return (ERROR);
		i++;
	}
	return (0);
}

static int 			get_map_size(int fd)
{
	char 			*line;
	int 			s;

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

t_game				*create_game(char *path_to_map)
{
	t_game			*g;
	int				fd;

	if (!(g = (t_game*)malloc(sizeof(t_game))))
		return (NULL);
	if (((fd = open(path_to_map, O_RDONLY)) < 1 || 
			(g->rows = get_map_size(fd)) == ERROR))
	{
		ft_putstr("File not found: ");
		ft_putendl(path_to_map);
		destroy_game(g);
		return (NULL);
	}
	close(fd);
	g->elem = (int*)malloc(sizeof(int) * g->rows);
	if(!(g->w_map = read_and_save_map(g->rows, path_to_map, g->elem)))
	{
		error_message("Wrong file or file is broken");
		destroy_game(g);
		return (NULL);
	}
	return (g);
}

t_player			*create_player()
{
	t_player		*p;

	if(!(p = (t_player*)malloc(sizeof(t_player))))
		return (NULL);
	p->dir.x = -1;
	p->dir.y = 0;
	p->plane.x = 0;
	p->plane.y = 0.86;
	p->plus.cos = cos(ROTATE);
	p->plus.sin = sin(ROTATE);
	p->minus.cos = cos(-ROTATE);
	p->minus.sin = sin(-ROTATE);
	p->speed = 0.21;
	return (p);
}