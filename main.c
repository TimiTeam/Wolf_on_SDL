#include "head.h"


int				main(int argc, char **argv)
{
	int			ret;
	t_sdl		*sdl;
	t_player	*player;
	t_game		*game;

	if (argc != 2)
		return (error_exit("need enter path to world map", NULL, NULL, NULL));
	if (!(sdl = new_t_sdl(1024, 720)))
		return (error_exit("FATAL", NULL, NULL, NULL));
	sdl->path_map = ft_strdup(argv[1]);
	if (init_objects(sdl, &player, &game))
		return (error_exit("FATAL ERROR", sdl, player, game));
	while ((ret = show_menu(sdl)))
	{
		if (ret == NEW_GAME)
		{
			free(player);
			player = create_player();
			find_free_place(game, &player->pos);
		}
		if (!game_loop(sdl, player, game))
			break ;
	}
	error_exit(NULL, sdl, player, game);
	return (0);
}