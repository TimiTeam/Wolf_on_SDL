/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_worker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:51:54 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/14 19:53:43 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void			free_void_map(void **map, int size)
{
	int			i;

	i = 0;
	if (!map)
		return ;
	while (i < size)
	{
		free(map[i]);
		map[i++] = NULL;
	}
	free(map);
	map[i] = NULL;
}

int				arr_size(char **arr)
{
	int			s;

	s = 0;
	while (arr[s])
		s++;
	return (s);
}

static int		*get_size_and_save(char *line, int *size_row)
{
	int			j;
	int			*dots;
	char		**ch_dost;

	if (ft_strchr(line, ' '))
		ch_dost = ft_strsplit(line, ' ');
	else if (ft_strchr(line, ','))
		ch_dost = ft_strsplit(line, ',');
	else
		return (0);
	*size_row = arr_size(ch_dost);
	dots = (int*)ft_memalloc(sizeof(int) * *size_row);
	j = 0;
	while (j < *size_row)
	{
		dots[j] = ft_atoi(ch_dost[j]);
		j++;
	}
	free_void_map((void**)ch_dost, *size_row);
	return (dots);
}

int				**read_and_save_map(int size, char *pth, int *rows_size)
{
	int			**arr;
	char		*line;
	int			fd;
	int			i;

	i = 0;
	if ((fd = open(pth, O_RDONLY)) < 1)
		return (NULL);
	arr = (int**)ft_memalloc(sizeof(int*) * size);
	while (get_next_line(fd, &line) > 0)
	{
		arr[i] = get_size_and_save(line, &rows_size[i]);
		ft_strdel(&line);
		i++;
	}
	close(fd);
	ft_strdel(&line);
	return (arr);
}
