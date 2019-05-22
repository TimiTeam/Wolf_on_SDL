/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:46:55 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/14 19:47:05 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int				main(int argc, char **argv)
{
	int			ret;
	t_sdl		*sdl;
	t_player	*player;
	t_game		*game;

	if (!(sdl = new_t_sdl(1024, 720)))
		return (error_message("FATAL", NULL, NULL, NULL));
	if (init_objects(sdl, &player, &game) == ERROR)
		return (1);
	ret = MENU;
	while (ret != EXIT)
	{
		if (ret == MENU)
			ret = show_menu(sdl);
		if (ret == NEW_GAME)
		{
			Mix_HaltMusic();
			if (new_clear_objects(sdl, player, game) == ERROR)
				return (1);
		}
		if (ret != EXIT)
			ret = game_loop(sdl, player, game);
	}
	exit_x (sdl, player, game);
	return (0);
}
