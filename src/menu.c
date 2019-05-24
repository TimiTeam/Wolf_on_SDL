/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbujalo <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:22:01 by tbujalo           #+#    #+#             */
/*   Updated: 2019/05/14 19:24:20 by tbujalo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "head.h"

void				render_manipulations(SDL_Renderer *ren, SDL_Texture *tex)
{
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);
	SDL_DestroyTexture(tex);
}

static int			return_key(t_sdl *s, int *choice)
{
	Mix_PlayChannel(-1, s->menu_select, 0);
	if (*choice == 1 && s->in_game)
		return (CONTINUE);
	else if (*choice == 0)
	{
		s->map = 0;
		return (NEW_GAME);
	}
	return (OK);
}

static int			menu_hook(SDL_Keycode k, t_sdl *s, int *choice)
{
	if (k == SDLK_RETURN)
		return (return_key(s, &(*choice)));
	Mix_PlayChannel(-1, s->menu_move, 0);
	if (k == SDLK_UP)
	{
		--(*choice);
		if (*choice < 0)
			*choice = 2;
		return (OK);
	}
	else if (k == SDLK_DOWN)
	{
		++(*choice);
		if (*choice > 2)
			*choice = 0;
		return (OK);
	}
	return (OK);
}

int					show_menu(t_sdl *s)
{
	SDL_Event		e;
	SDL_Texture		*tex;
	int				choice;
	int				res;

	choice = 0;
	while (1)
	{
		tex = SDL_CreateTextureFromSurface(s->ren, s->menu->walls[choice]);
		render_manipulations(s->ren, tex);
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				return (EXIT);
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE ||
				(e.key.keysym.sym == SDLK_RETURN && choice == 2))
					return (EXIT);
				if ((res = menu_hook(e.key.keysym.sym, s, &choice)) != OK)
					return (res);
			}
		}
	}
}
