/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 19:33:03 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/26 19:33:16 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int				map_error(char *mes)
{
	ft_putendl(mes);
	return (ERROR);
}

static int		empty_row(int num_row)
{
	ft_putstr("Empty line at row: ");
	ft_putnbr(num_row + 1);
	return (map_error(""));
}

static int		first_and_last_row(int *row, int size, int row_num)
{
	int			i;

	if (size == 0)
		return (empty_row(row_num));
	if (size < 3)
		return (map_error("Missing free place"));
	i = 0;
	while (i < size)
		if (row[i++] == 0)
			return (map_error("Missing the wall on perimeter"));
	return (OK);
}

static int		check_line(int *line, int size, int *emp, int num_row)
{
	int			i;

	if (size == 0)
		return (empty_row(num_row));
	if (line[0] == 0 || line[size - 1] == 0)
		return (map_error("Missing the wall on perimeter"));
	if (size < 3)
		return (map_error("Missing free place"));
	i = 0;
	while (i < size)
		if (line[i++] == 0)
			(*emp)++;
	return (i);
}

int				valid_map(int **map, int size, int *rows_size)
{
	int			j;
	int			prv_row_size;
	int			emp;

	if (size <= 1)
		return (map_error("Too small map"));
	j = 0;
	emp = 0;
	if (first_and_last_row(map[j], rows_size[j], j) == ERROR)
		return (ERROR);
	j++;
	while (j < size - 1)
	{
		if ((prv_row_size = check_line(map[j], rows_size[j], &emp, j)) == ERROR)
			return (ERROR);
		if (rows_size[j - 1] != prv_row_size)
			return (map_error("The length of rows not the same"));
		j++;
	}
	if (first_and_last_row(map[j], rows_size[j], j) == ERROR)
		return (ERROR);
	if (emp < 1)
		return (map_error("Missing free place"));
	return (OK);
}
