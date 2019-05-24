/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 18:30:14 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/24 19:06:48 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

int				run_raycasting_threads(t_sdl *s, t_player *p, t_game *g)
{
	t_data		data[THREADS];
	pthread_t	thread[THREADS];
	int			i;
	int			step;

	step = s->win_size.x / THREADS - 1;
	i = 0;
	while (i < THREADS)
	{
		create_and_fill(&data[i], s, i * step, step);
		data[i].player = p;
		data[i].game = g;
		pthread_create(&thread[i], NULL, build_walls, &data[i]);
		i++;
	}
	i = 0;
	while (i < THREADS)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
	return (OK);
}

int				the_end_screen(t_sdl *s)
{
	SDL_Texture	*tex;
	SDL_Surface	*suf;

	suf = load_surface("res/the_end.bmp");
	tex = SDL_CreateTextureFromSurface(s->ren, suf);
	SDL_RenderCopy(s->ren, tex, NULL, NULL);
	SDL_RenderPresent(s->ren);
	SDL_Delay(1200);
	SDL_FreeSurface(suf);
	SDL_DestroyTexture(tex);
	return (EXIT);
}

static int		game_hook(SDL_Event e, t_sdl *s, t_player *p, t_game *g)
{
	int			ret;

	if (e.type == SDL_QUIT)
		return (EXIT);
	else if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_ESCAPE)
			return (EXIT);
		if (e.key.keysym.sym == SDLK_m)
			return (MENU);
		else if ((ret = make_actions(e.key.keysym.sym, s, p, g)) == THE_END)
			return (the_end_screen(s));
		else
			return (ret);
	}
	return (OK);
}

int				game_loop(t_sdl *s, t_player *p, t_game *g)
{
	SDL_Event	e;
	SDL_Texture	*tex;
	int			stop;

	stop = 0;
	if (Mix_PlayingMusic() == 0)
		Mix_PlayMusic(s->music, -1);
	else if (Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
	while (!stop)
	{
		s->start = clock();
		run_raycasting_threads(s, p, g);
		tex = SDL_CreateTextureFromSurface(s->ren, s->img->surf);
		render_manipulations(s->ren, tex);
		s->end = clock();
		p->speed = (double)(s->end - s->start) / CLOCKS_PER_SEC * 4.5;
		while (SDL_PollEvent(&e))
			if ((stop = game_hook(e, s, p, g)) != OK)
				break ;
	}
	Mix_PauseMusic();
	return (stop);
}
