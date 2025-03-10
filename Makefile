NAME:=				minirt

HEADERS_DIRECTORY:=		include
HEADERS:=			$(HEADERS_DIRECTORY)/minirt.h

SOURCES_DIRECTORY:=		src
SOURCES:=			$(SOURCES_DIRECTORY)/main.c \
					$(SOURCES_DIRECTORY)/parsing/parser.c \
					$(SOURCES_DIRECTORY)/parsing/factories.c \
					$(SOURCES_DIRECTORY)/parsing/factories_utils.c \
					$(SOURCES_DIRECTORY)/parsing/checker.c \
					$(SOURCES_DIRECTORY)/render/render.c \
					$(SOURCES_DIRECTORY)/exit/error.c \
					$(SOURCES_DIRECTORY)/math/math.c \
					$(SOURCES_DIRECTORY)/render/shading.c \
					$(SOURCES_DIRECTORY)/multithreading/worker.c \
					$(SOURCES_DIRECTORY)/bvh/bvh.c \
					$(SOURCES_DIRECTORY)/mlx-hooks/mlx.c \
					$(SOURCES_DIRECTORY)/mlx-hooks/events.c \
					$(wildcard $(SOURCES_DIRECTORY)/intersection_tests/*.c) \
					$(SOURCES_DIRECTORY)/render/checkerboard.c \
					$(SOURCES_DIRECTORY)/math/find_median.c \
					$(SOURCES_DIRECTORY)/math/dot_33.c \
					$(SOURCES_DIRECTORY)/math/rodrigues.c


OBJECTS_DIRECTORY:=		build
# OBJECTS:=			$(patsubst $(SOURCES_DIRECTORY)/%.c,$(OBJECTS_DIRECTORY)/%.o, $(SOURCES))
OBJECTS:=			$(patsubst $(SOURCES_DIRECTORY)/%.c,$(SOURCES_DIRECTORY)/%.o, $(SOURCES))

LIBFT_DIRECTORY:=		lib/libft
LIBFT:=				$(LIBFT_DIRECTORY)/libft.a

MLX_DIRECTORY:=			lib/minilibx-linux
LIBMLX:=			$(MLX_DIRECTORY)/libmlx.a

LIB_FLAGS:=			-L$(LIBFT_DIRECTORY) -lft -L$(MLX_DIRECTORY) -lmlx_Linux -lXext -lX11 -lm

CC:=				gcc
CFLAGS:=			-Wuninitialized -Wall -Wextra -g -I$(HEADERS_DIRECTORY) -I$(LIBFT_DIRECTORY) -I$(MLX_DIRECTORY) -mavx -Ofast -march=native -ffunction-sections -Wl,--gc-sections -flto -finline-limit=2000

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
	@rm -rf $(OBJECTS)
#	@rm -rf $(MLX_DIRECTORY) clean >/dev/null 2>&1
	@make -s -C $(LIBFT_DIRECTORY) fclean >/dev/null 2>&1

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

