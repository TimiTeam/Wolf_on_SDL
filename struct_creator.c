#include "head.h"


int			check_line(char *l)
{
	int		i;

	i = 0;
	while (l[i])
	{
		if ((l[i] < '0' || l[i] > '9') && l[i] != ' ' && l[i] != ',')
			return (ERROR);
		i++;
	}
	return (0);
}

int 		get_map_size(int fd)
{
	char 	*line;
	int 	s;

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

t_sdl			*new_t_sdl(int s_x, int s_y, char *file_pth)
{
	t_sdl		*s;
	int			fd;
	int 		i;

	if(!(s = (t_sdl*)malloc(sizeof(t_sdl))))
		return (NULL);
	s->win_size.x = s_x;
	s->win_size.y = s_y;
	if ((fd = open(file_pth, O_RDONLY)) < 1)
	{
		error_message("File not found");
		free(s);
		return (NULL);
	}
	if ((s->rows = get_map_size(fd)) == ERROR)
	{
		error_message("Wrong file or file is broken");
		free(s);
		return (NULL);
	}
	close(fd);
	s->elem = (int*)malloc(sizeof(int) * s->rows);
	if(!(s->w_map = read_and_save_map(s->rows, file_pth, s->elem)))
	{
		error_message("Wrong file or file is broken");
		free(s);
		return (NULL);
	}
	s->win = NULL;
	s->ren = NULL;
	s->surf = NULL;
	i = 0;
	while (i < COUNT_TEXT)
		s->walls[i++] = NULL;
	return (s);
}

t_game			*create_game()
{
	t_game		*g;

	if(!(g = (t_game*)malloc(sizeof(t_game))))
		return (NULL);
	return (g);
}

t_player		*create_player()
{
	t_player	*p;

	if(!(p = (t_player*)malloc(sizeof(t_player))))
		return (NULL);
	p->pos.x = 22;
	p->pos.y = 12;
	p->dir.x = -1;
	p->dir.y = 0;
	p->plane.x = 0;
	p->plane.y = 0.86;
	return (p);
}

SDL_Surface		*load_surface(SDL_Renderer *ren, char *pth)
{
	SDL_Surface	*opt;
	SDL_Surface	*s;

	opt = NULL;
	if((s = SDL_LoadBMP(pth)))
	{
		return (s);
		opt = SDL_ConvertSurface(s, s->format, 0);
		SDL_FreeSurface(s);
	}
	return (opt);
}

int				load_walls(char *file_list, t_sdl *sdl)
{
	int			i;
	int			fd;
	char		*name;
	char		*pref;

	if ((fd = open(file_list, O_RDONLY)) < 1)
		return (ERROR);
	i = 0;
	while (get_next_line(fd, &name) > 0)
	{
		pref = ft_strjoin("res/",name);
		if(!(sdl->walls[i] = load_surface(sdl->ren, pref)))
			return (ERROR);
		ft_strdel(&pref);
		ft_strdel(&name);
		i++;
	}
	ft_strdel(&name);
	ft_strdel(&pref);
	if (i != COUNT_TEXT)
		return (ERROR);
	return (0);
}

int				init_sdl_elem(t_sdl *s)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (1);
	s->win = SDL_CreateWindow("test_sdl_wolf", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, s->win_size.x, s->win_size.y, SDL_WINDOW_SHOWN);
	if (!s->win)
		return (1);
	s->ren = SDL_CreateRenderer(s->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!s->ren)
		return (1);
//	s->surf = SDL_CreateRGBSurface(0, s->win_size.x, s->win_size.y, 32, 0, 0 ,0, 255);
	s->surf = NULL;
	if (load_walls("res/resurses.txt", s) == ERROR)
		return (1);
	SDL_SetRenderDrawColor(s->ren, 255, 255, 255, 255);
	return (0);
}
