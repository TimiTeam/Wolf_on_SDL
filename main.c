#include "head.h"

int				main(int argc, char **argv)
{
	int			ret;
	t_sdl		*sdl;
	t_game		*game;

	if (argc != 2)
		return (error_exit("need enter path to world map", NULL, NULL));
	if (!(sdl = new_t_sdl(980, 640)))
		return (error_exit("FATAL", NULL, NULL));
	sdl->path_map = ft_strdup(argv[1]);
	if (init_sdl_elem(sdl) == ERROR)
		return (error_exit(SDL_GetError(), sdl, NULL));
	if(!(game = create_game(sdl->path_map)))
		return (error_exit("Failed to create game :(", sdl, game));
	game->half_win_y = sdl->win_size.y / 2;
	while ((ret = start_game(sdl, game)))
	{
		clear_fields_t_game(game);
		if (ret == ERROR)
			return (error_exit(SDL_GetError(), sdl, game));
	}
	close_all(sdl);
	return (0);
}
