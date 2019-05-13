#include "head.h"


int				main(int argc, char **argv)
{
	int			ret;
	t_sdl		*sdl;
	t_player	*player;
	t_game		*game;

	if (!(sdl = new_t_sdl(1024, 720)))
		return (error_message("FATAL"));
	if (init_sdl_elem(sdl) == ERROR)
		return (error_message(SDL_GetError()));
	ret = MENU;
	while (ret)
	{
		if (ret == MENU)
			ret = show_menu(sdl);
		if (ret == NEW_GAME)
		{
			error_exit(NULL, NULL, player, game);
			sdl->map = sdl->map >= sdl->count_maps ? 0 : sdl->map;
			if(init_objects(sdl, &player, &game) == ERROR)
				return (error_exit("ERROR", sdl, player, game));
		}
		if (ret)
			ret = game_loop(sdl, player, game);
	}
	error_exit(NULL, sdl, player, game);
	return (0);
}