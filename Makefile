NAME := minirt

HEADERS_DIRECTORY := include
HEADERS := $(HEADERS_DIRECTORY)/minirt.h

SOURCES_DIRECTORY := src
OBJECTS_DIRECTORY := build

SOURCES := $(SOURCES_DIRECTORY)/main.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/identifiers.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/identifiers2.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/parser.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/factories.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/factories2.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/factories3.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/factories_utils.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/factories_utils2.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/factories_utils3.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/checker.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/init.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/obj_file.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/rt_file.c
SOURCES += $(SOURCES_DIRECTORY)/parsing/counter.c
SOURCES += $(SOURCES_DIRECTORY)/bvh/bvh_bbox.c
SOURCES += $(SOURCES_DIRECTORY)/bvh/bvh_geodata.c
SOURCES += $(SOURCES_DIRECTORY)/bvh/create_bvh.c
SOURCES += $(SOURCES_DIRECTORY)/bvh/create_bvh_utils.c
SOURCES += $(SOURCES_DIRECTORY)/bvh/cut_in_two.c
SOURCES += $(SOURCES_DIRECTORY)/bvh/update_group.c
SOURCES += $(SOURCES_DIRECTORY)/exit/prints.c
SOURCES += $(SOURCES_DIRECTORY)/exit/error.c
SOURCES += $(SOURCES_DIRECTORY)/exit/free.c
SOURCES += $(SOURCES_DIRECTORY)/render/render.c
SOURCES += $(SOURCES_DIRECTORY)/render/shoot_ray.c
SOURCES += $(SOURCES_DIRECTORY)/render/shading.c
SOURCES += $(SOURCES_DIRECTORY)/render/checkerboard.c
SOURCES += $(SOURCES_DIRECTORY)/render/shoot_refrac_reflec.c
SOURCES += $(SOURCES_DIRECTORY)/render/whitted.c
SOURCES += $(SOURCES_DIRECTORY)/multithreading/worker.c
SOURCES += $(SOURCES_DIRECTORY)/mlx-hooks/mlx.c
SOURCES += $(SOURCES_DIRECTORY)/mlx-hooks/events.c
SOURCES += $(SOURCES_DIRECTORY)/mlx-hooks/mouse.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/aabb_simd.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/aabb_simd_fast.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/bvh.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/cylinder.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/intersection_test_shadow.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/intersection_test_visibility.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/plane.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/sphere.c
SOURCES += $(SOURCES_DIRECTORY)/intersection_tests/triangle.c
SOURCES += $(SOURCES_DIRECTORY)/math/math.c
SOURCES += $(SOURCES_DIRECTORY)/math/math_utils.c
SOURCES += $(SOURCES_DIRECTORY)/math/find_median.c
SOURCES += $(SOURCES_DIRECTORY)/math/find_median2.c
SOURCES += $(SOURCES_DIRECTORY)/math/dot_in_place.c
SOURCES += $(SOURCES_DIRECTORY)/math/rodrigues.c

OBJECTS := $(patsubst $(SOURCES_DIRECTORY)/%.c,$(OBJECTS_DIRECTORY)/%.o, $(SOURCES))

LIBFT_DIRECTORY := lib/libft
LIBFT := $(LIBFT_DIRECTORY)/libft.a

MLX_DIRECTORY := lib/minilibx-linux
LIBMLX := $(MLX_DIRECTORY)/libmlx.a

LIB_FLAGS := -L$(LIBFT_DIRECTORY)
LIB_FLAGS += -L$(MLX_DIRECTORY)    
LIB_FLAGS += -lft
LIB_FLAGS += -lmlx_Linux
LIB_FLAGS += -lXext
LIB_FLAGS += -lX11
LIB_FLAGS += -lm

CC := cc

CFLAGS := -Wall -Wextra -Werror
CFLAGS += -g
CFLAGS += -Wuninitialized
CFLAGS += -mavx
CFLAGS += -Ofast
CFLAGS += -mavx
CFLAGS += -march=native
CFLAGS += -ffunction-sections
CFLAGS += -flto
CFLAGS += -finline-functions
CFLAGS += -I$(HEADERS_DIRECTORY) -I$(LIBFT_DIRECTORY) -I$(MLX_DIRECTORY)

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

$(NAME): $(OBJECTS) $(LIBFT) $(LIBMLX)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIB_FLAGS) -o $(NAME) -Wl,--gc-sections

$(OBJECTS_DIRECTORY)/%.o: $(SOURCES_DIRECTORY)/%.c $(HEADERS) | $(OBJECTS_DIRECTORY)
	@mkdir -p $(dir $@)
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
