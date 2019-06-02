NAME := wolf3d

SRCS	:=	main.c struct_creator.c struct_free_and_exit.c game_loop.c \
			map_worker.c data_and_image.c pixel_worker.c actions.c init_objects.c \
			menu.c player.c load_sound.c texture_choice.c drawing_functions.c \
			calculate_wall.c map_validator.c

DIR_SRC := $(CURDIR)/src
DIR_OBJ := $(CURDIR)/obj


OBJS := $(SRCS:.c=.o)

OBJS := $(addprefix $(DIR_OBJ)/, $(OBJS))
SRCS := $(addprefix $(DIR_SRC)/, $(SRCS))

CC := clang

SDL_INCL =	-I $(CURDIR)/frameworks/SDL2.framework/Headers/ \
			-I $(CURDIR)/frameworks/SDL2_mixer.framework/Headers

FLAG_W = -Wall -Wextra -Werror

FLAG_F = -F frameworks

LFT_INCL = -I libft/ 

LIBFT_A = libft/libft.a

SDL_RUN_FLAGS = -rpath frameworks -framework SDL2 -framework SDL2_mixer

all: $(NAME)

$(LIBFT_A):
	@make -C libft/

$(DIR_OBJ):
	@mkdir -p $(DIR_OBJ)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(OBJS) $(FLAG_F) $(SDL_RUN_FLAGS) -L libft -lft -o $(NAME)


$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c | $(DIR_OBJ)
	$(CC) $(FLAG_W) -pthread  $(FLAG_F) $(SDL_INCL) $(LFT_INCL) -c $< -o $@

clean:
	@make -C libft/ clean
	/bin/rm -rf $(OBJS)
	/bin/rm -rf $(DIR_OBJ)

fclean: clean
	@make -C libft/ fclean
	/bin/rm -rf $(NAME)

re: fclean all 

.PHONY: all fclean clean re
