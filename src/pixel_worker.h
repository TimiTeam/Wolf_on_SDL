/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_worker.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 18:48:56 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/14 18:50:20 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PIXEL_WORKER_H
# define __PIXEL_WORKER_H

# include <SDL.h>

typedef	struct		s_point
{
	int				x;
	int				y;
}					t_point;

Uint32				get_pixel(SDL_Surface *src, t_point pos);
void				put_pixel(SDL_Surface *dst, t_point pos, Uint32 pixel);
int					scale_image(SDL_Surface *dst, SDL_Surface *src);

#endif
