/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_and_image.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:02:38 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/14 19:05:20 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

SDL_Surface		*load_surface(char *pth)
{
	SDL_Surface	*opt;
	SDL_Surface	*s;

	opt = NULL;
	if ((s = SDL_LoadBMP(pth)))
	{
		opt = SDL_ConvertSurface(s, s->format, 0);
		SDL_FreeSurface(s);
	}
	return (opt);
}

t_images		*new_t_images(void)
{
	t_images	*i;
	int			y;

	i = (t_images*)ft_memalloc(sizeof(t_images));
	i->surf = NULL;
	y = 0;
	while (y <= CO_TEXT)
		i->walls[y++] = NULL;
	return (i);
}

t_images		*create_and_images(char *file_list)
{
	int			i;
	int			fd;
	char		*name;
	t_images	*img;

	if ((fd = open(file_list, O_RDONLY)) < 1)
	{
		ft_putstr("FATAL! File not found:");
		ft_putendl(file_list);
		return (NULL);
	}
	i = -1;
	img = new_t_images();
	while (get_next_line(fd, &name) > 0)
	{
		if (!(img->walls[++i] = load_surface(name)))
		{
			clear_image(img);
			img = NULL;
			break ;
		}
		ft_strdel(&name);
	}
	ft_strdel(&name);
	return (img);
}

void			create_and_fill(t_data *d, t_sdl *s, int star_x, int step)
{
	d->img = s->img;
	d->win_size = s->win_size;
	d->half_win_y = s->win_size.y >> 1;
	d->start_x = star_x;
	d->end_x = star_x + step;
}
