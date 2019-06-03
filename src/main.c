/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:46:55 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/26 20:36:14 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int				not_found(char *mess)
{
	ft_putstr("File not found: ");
	ft_putendl(mess);
	return (ERROR);
}

int				close_exit(char **line, int fd)
{
	not_found(*line);
	ft_strdel(&(*line));
	close(fd);
	return (ERROR);
}

int				opening_resources(int file, char *name)
{
	char		*line;
	int			fd;
	int			i;

	i = 0;
	while (get_next_line(file, &line) > 0)
	{
		if ((fd = open(line, O_RDONLY)) < 1)
			return (close_exit(&line, fd));
		i++;
		close(fd);
		ft_strdel(&line);
	}
	if (!ft_strcmp(name, RES_IMG) && i != CO_TEXT)
		return (map_error("Missing textures"));
	else if (!ft_strcmp(name, RES_MAPS) && i != COUNT_MAPS)
		return (map_error("Wrong count of the maps"));
	return (OK);
}

int				checking_resources(void)
{
	int			fd;

	if ((fd = open(RES_IMG, O_RDONLY)) < 1)
		return (not_found(RES_IMG));
	if (opening_resources(fd, RES_IMG) == ERROR)
		return (ERROR);
	close(fd);
	if ((fd = open(RES_MENU, O_RDONLY)) < 1)
		return (not_found(RES_MENU));
	if (opening_resources(fd, RES_MENU) == ERROR)
		return (ERROR);
	close(fd);
	if ((fd = open(RES_MAPS, O_RDONLY)) < 1)
		return (not_found(RES_MAPS));
	if (opening_resources(fd, RES_MAPS) == ERROR)
		return (ERROR);
	close(fd);
	return (OK);
}

int				main(void)
{
	int			ret;
	t_sdl		*sdl;
	t_player	*player;
	t_game		*game;

	if (checking_resources() == ERROR || !(sdl = new_t_sdl(1024, 720)))
		return (error_message("FATAL", NULL, NULL, NULL));
	if (init_objects(sdl, &player, &game) == ERROR)
		return (ERROR);
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
	exit_x(sdl, player, game);
	return (0);
}
