#include "head.h"

int				main(int argc, char **argv)
{
	int			ret;
	t_sdl		*sdl;

	if (argc != 2)
		return (error_exit("need enter path to world map", NULL));
	if (!(sdl = new_t_sdl(1200, 980, argv[1])))
		return (error_exit("FATAL", NULL));
	if (init_sdl_elem(sdl))
		return (error_exit(SDL_GetError(), sdl));
	while ((ret = start_game(sdl)))
	{
		if (ret == ERROR)
			return (error_exit(SDL_GetError(), sdl));
	}
	close_all(sdl);
	return (0);
}