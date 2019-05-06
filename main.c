#include "head.h"

void 		find_free_place(t_game *s, t_vec *pos)
{
	int 	i;
	int 	j;

	i = 0;
	while (i < s->rows)
	{
		j = 0;
		while (j < s->elem[i])
		{
			if (s->w_map[i][j] == 0 )
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

int				init_objects(t_sdl *sdl)
{
	t_game		*game;
	t_player	*player;

	if(!(game = create_game(sdl->path_map)))
		return (error_message("Failed to create game :("));
	sdl->game = game;
	if (!(player = create_player()))
		return (error_message("Create player"));
	find_free_place(game, &player->pos);
	sdl->player = player;
	return (OK);
}

int				main(int argc, char **argv)
{
	int			ret;
	t_sdl		*sdl;

	if (argc != 2)
		return (error_exit("need enter path to world map", NULL, NULL, NULL));
	if (!(sdl = new_t_sdl(1024, 720)))
		return (error_exit("FATAL", NULL, NULL, NULL));
	sdl->path_map = ft_strdup(argv[1]);
	if (init_sdl_elem(sdl) == ERROR)
		return (error_exit(SDL_GetError(), sdl, NULL, NULL));
	if (init_objects(sdl))
		return (error_exit("FATAL ERROR", sdl, sdl->game, sdl->player));
	while ((ret = start_game(sdl, sdl->game, sdl->player)))
	{
		if (ret == NEW_GAME)
		{
			free(sdl->player);
			destroy_game(sdl->game);
			if (init_objects(sdl))
				return (error_exit("FATAL ERROR", sdl, sdl->game, sdl->player));
		}
	}
	free(sdl->player);
	destroy_game(sdl->game);
	close_all(sdl);
	return (0);
}
