NAME = test_sdl_wolf

SRCS = main.c struct_creator.c struct_free_and_exit.c game_loop.c map_worker.c data_and_image.c pixel_worker.c actions.c init_objects.c

OBJS = $(SRCS:.c=.o)

SDL_INCL = -I frameworks/SDL2.framework/Headers/ 

SDL_IMG_INCL = -I framework/SDL2_image.framework/Headers/

SDL_INCL_LINUX = `sdl2-config --cflags --libs`

LFT_INCL = -I libft/ -L libft -lft

LIBFT_A = libft/libft.a

SDL_RUN_FLAGS = -framework SDL2 -rpath frameworks

all: $(NAME)

compile: $(SRCS) $(LIBFT_A)
	clang -g -pthread $(SRCS) $(SDL_INCL) $(SDL_RUN_FLAGS) $(LFT_INCL) -o $(NAME)

compile_linux: $(SRCS) $(LIBFT_A)
	clang -g $(SRCS) $(SDL_INCL_LINUX) $(LFT_INCL) -lm -o $(NAME)

compile_lib: libft/
	make -C libft/

$(NAME): compile_lib compile

clean:
	make -C libft/ clean
	/bin/rm -f $(OBJS)

fclean: clean
	make -C libft/ fclean
	/bin/rm -f $(NAME)

re: fclean all 
