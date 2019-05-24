/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_sound.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 17:52:23 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 17:52:44 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

Mix_Music		*load_music(char *path)
{
	Mix_Music	*music;

	if (!(music = Mix_LoadMUS(path)))
		ft_putendl(Mix_GetError());
	return (music);
}

Mix_Chunk		*load_effects(char *path)
{
	Mix_Chunk	*eff;

	if (!(eff = Mix_LoadWAV(path)))
		ft_putendl(Mix_GetError());
	return (eff);
}
