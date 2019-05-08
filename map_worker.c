#include "head.h"

int 				arr_size(char **arr)
{
	int		s;

	s = 0;
	while (arr[s])
		s++;
	return (s);
}

static int 			*get_size_and_save(char *line, int *size_row)
{
	int 	j;
	int 	*dots;
	char 	**ch_dost;

	if (ft_strchr(line, ' '))
		ch_dost = ft_strsplit(line, ' ');
	else if (ft_strchr(line, ','))
		ch_dost = ft_strsplit(line, ',');
	else
		return (0);
	*size_row = arr_size(ch_dost);
	dots = (int*)malloc(sizeof(int) * *size_row);
	j = 0;
	while (j < *size_row)
	{
		dots[j] = ft_atoi(ch_dost[j]);
		j++;
	}
	free_void_map((void*)ch_dost, *size_row);
	return (dots);
}

int 				**read_and_save_map(int size, char *pth, int *rows_size)
{
	int 	**arr;
	char 	*line;
	int 	fd;
	int 	i;

	arr = (int**)malloc(sizeof(int*) * size);
	i = 0;
	if ((fd = open(pth, O_RDONLY)) < 1)
		return (NULL);
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