#include "head.h"


void 			find_free_place(t_game *s, t_vec *pos)
{
	int 		i;
	int 		j;

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
	if((rd = read(fd, &buf, 256)) < 1)
	{
		ft_putstr("File is broken\n");
		return (NULL);
	}
	buf[rd] = '\0';
	return(ft_strsplit(buf, '\n'));
}

int				init_sdl_elem(t_sdl *s)
{
	int			flag;

	flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return (ERROR);
	s->win = SDL_CreateWindow("test_sdl_wolf", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, s->win_size.x, s->win_size.y, SDL_WINDOW_SHOWN);
	if (!s->win)
		return (ERROR);
	s->ren = SDL_CreateRenderer(s->win, -1, flag);
	if (!s->ren)
		return (ERROR);
	if (!(s->img = create_and_images("res/res.txt", s->ren)))
		return (ERROR);
	if (!(s->menu = create_and_images("res/menu/list.txt", s->ren)))
		return (ERROR);
	if (!(s->maps = read_maps_path("maps/maps.txt")))
		return (ERROR);
	s->count_maps = arr_size(s->maps);
	s->menu->surf = NULL;
	return (0);
}

int				init_objects(t_sdl *s, t_player **p, t_game **g)
{
	t_vec		pos;

	*p = NULL;
	*g = NULL;
	if(!(*g = create_game(s->maps[s->map])))
		return (error_message("Failed to create game :("));
	(*g)->floor = s->map == s->count_maps - 1 ? COUNT_TEXT - 1 : COUNT_TEXT - 3;
	(*g)->ceiling = s->map == s->count_maps - 1 ? COUNT_TEXT - 2 : COUNT_TEXT - 4;
	if (!(*p = create_player()))
		return (error_message("Create player"));
	find_free_place(*g, &pos);
	(*p)->pos = pos;
	return (OK);
}