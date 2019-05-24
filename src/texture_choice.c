/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_choice.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 19:16:22 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 19:17:46 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

static int		get_side_texture(t_data *d)
{
	if (d->side == 0 && d->ray.x < 0)
		return (0);
	else if (d->side == 0 && d->ray.x > 0)
		return (1);
	else if (d->side == 1 && d->ray.y > 0)
		return (2);
	else if (d->side == 1 && d->ray.y < 0)
		return (3);
	else
		return (CO_TEXT - 1);
}

int				get_num_texture(t_data *d, int map_tex)
{
	if (map_tex >= CO_TEXT ||
			map_tex == 0)
		return (get_side_texture(d));
	return (map_tex);
}
