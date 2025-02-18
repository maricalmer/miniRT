NAME:=				minirt

HEADERS_DIRECTORY:=		include
HEADERS:=			$(HEADERS_DIRECTORY)/minirt.h

SOURCES_DIRECTORY:=		src
SOURCES:=			$(SOURCES_DIRECTORY)/main.c \
					$(SOURCES_DIRECTORY)/parsing.c \
					$(SOURCES_DIRECTORY)/render.c \
					$(SOURCES_DIRECTORY)/math.c \
					$(SOURCES_DIRECTORY)/shading.c \
					$(SOURCES_DIRECTORY)/worker.c \
					$(SOURCES_DIRECTORY)/bvh.c \
					$(SOURCES_DIRECTORY)/mlx.c \
					$(SOURCES_DIRECTORY)/intersect.c

OBJECTS_DIRECTORY:=		build
OBJECTS:=			$(patsubst $(SOURCES_DIRECTORY)/%.c,$(OBJECTS_DIRECTORY)/%.o, $(SOURCES))

LIBFT_DIRECTORY:=		lib/libft
LIBFT:=				$(LIBFT_DIRECTORY)/libft.a

MLX_DIRECTORY:=			lib/minilibx-linux
LIBMLX:=			$(MLX_DIRECTORY)/libmlx.a

LIB_FLAGS:=			-L$(LIBFT_DIRECTORY) -lft -L$(MLX_DIRECTORY) -lmlx_Linux -lXext -lX11 -lm

CC:=				cc
CFLAGS:=			-Wall -Wextra -g -I$(HEADERS_DIRECTORY) -I$(LIBFT_DIRECTORY) -I$(MLX_DIRECTORY) #-Ofast -march=native 

all: $(OBJECTS_DIRECTORY) $(LIBMLX) $(LIBFT) $(NAME)

$(LIBMLX):
	@if [ ! -d "$(MLX_DIRECTORY)" ]; then \
			mkdir -p $(MLX_DIRECTORY); \
			echo "Cloning MiniLibX repository..."; \
			git clone https://github.com/42Paris/minilibx-linux.git $(MLX_DIRECTORY); \
	fi
	@echo "Compiling MiniLibX..."
	@make -s -C $(MLX_DIRECTORY) >/dev/null 2>&1;
	@echo "MiniLibX compiled"

$(NAME):	$(OBJECTS) $(LIBFT) $(LIBMLX)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIB_FLAGS) -o $(NAME)

$(OBJECTS_DIRECTORY)/%.o:	$(SOURCES_DIRECTORY)/%.c $(HEADERS)
	@mkdir -p $(OBJECTS_DIRECTORY)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_DIRECTORY):
	@mkdir -p $(OBJECTS_DIRECTORY)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIRECTORY)

clean:
	@rm -rf $(OBJECTS_DIRECTORY)
	@rm -rf $(MLX_DIRECTORY) clean >/dev/null 2>&1
	@make -s -C $(LIBFT_DIRECTORY) fclean >/dev/null 2>&1

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

