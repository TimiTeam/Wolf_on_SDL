/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 19:17:54 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 19:18:19 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void			find_free_place(t_game *s, t_vec *pos)
{
	int			i;
	int			j;

	i = 0;
	while (i < s->rows)
	{
		j = 0;
		while (j < s->elem[i])
		{
			if (s->w_map[i][j] == 0)
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

int				fill_new_t_player(t_player *p)
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
