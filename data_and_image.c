#include "head.h"

SDL_Surface		*load_surface(SDL_Renderer *ren, char *pth)
{
	SDL_Surface	*opt;
	SDL_Surface	*s;

	opt = NULL;
	if((s = SDL_LoadBMP(pth)))
	{
		opt = SDL_ConvertSurface(s, s->format, 0);
		SDL_FreeSurface(s);
	}
	return (opt);
}

t_images		*new_t_images()
{
	t_images	*i;
	int			y;

	i = (t_images*)malloc(sizeof(t_images));
	i->surf = NULL;
	y = 0;
	while (y < COUNT_TEXT)
		i->walls[y++] = NULL;
	return (i);
}

t_images		*create_and_images(char *file_list, SDL_Renderer *ren)
{
	int			i;
	int			fd;
	char		*name;
	char		*pref;
	t_images	*img;

	if ((fd = open(file_list, O_RDONLY)) < 1)
	{
		ft_putstr("FATAL! File not found:");
		ft_putendl(file_list);
		return (NULL);
	}
	i = 0;
	img = new_t_images();
	while (get_next_line(fd, &name) > 0)
	{
		pref = ft_strjoin("res/",name);
		if(!(img->walls[i] = load_surface(ren, pref)))
			return (NULL);
		ft_strdel(&pref);
		ft_strdel(&name);
		i++;
	}
	ft_strdel(&name);
	ft_strdel(&pref);
	return (img);
}

t_data			*new_empty_data()
{
	t_data		*d;

	if(!(d = (t_data*)malloc(sizeof(t_data))))
		return (NULL);
	d->move.x = 0;
	d->move.y = 0;
	d->step.x = 0;
	d->step.y = 0;
	d->side = 0;
	d->wall_dist = 0;
	d->wall_size = 0;
	d->half_wall_size = 0;
	d->side_dist.x = 0;
	d->side_dist.y = 0;
	d->delta_dist.x = 0;
	d->delta_dist.y = 0;
	return (d);
}

t_data			*create_and_fill(t_sdl *s, int star_x, int step)
{
	t_data		*d;

	if (!(d = new_empty_data()))
		return (NULL);
	d->img = s->img;
	d->win_size = s->win_size;
	d->half_win_y = s->win_size.y / 2;
	d->start_x = star_x;
	d->end_x = star_x + step;
	return (d);
}
