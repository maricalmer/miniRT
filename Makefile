NAME := miniRT

ROOT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

# headers
HEADERS_DIRECTORY := $(ROOT_DIR)/include
HEADERS := $(addprefix $(HEADERS_DIRECTORY)/, \
		minirt.h data.h prototypes.h settings.h \
)

# source files
SOURCES_DIRECTORY := $(ROOT_DIR)/src
OBJECTS_DIRECTORY := $(ROOT_DIR)/build
SOURCES := $(addprefix $(SOURCES_DIRECTORY)/,\
		main.c \
		$(addprefix parsing/, identifiers.c identifiers2.c parser.c factories.c factories2.c factories3.c factories4.c \
			factories_utils.c factories_utils2.c factories_utils3.c checker.c init.c obj_file.c obj_file2.c rt_file.c \
			counter.c) \
		$(addprefix bvh/, bvh_bbox.c bvh_geodata.c create_bvh.c create_bvh_utils.c cut_in_two.c update_group.c) \
		$(addprefix exit/, error.c free.c free2.c) \
		$(addprefix logs/, prints.c prints2.c) \
		$(addprefix render/, render.c calculate_pixel.c shoot_ray.c shading.c checkerboard.c shoot_refrac_reflec.c \
			whitted.c) \
		$(addprefix multithreading/, worker.c) \
		$(addprefix mlx_hooks/, mlx.c events.c mouse.c) \
		$(addprefix intersection_tests/, aabb_simd.c aabb_simd_fast.c bvh.c cylinder.c intersection_test_shadow.c \
			intersection_test_visibility.c plane.c rectangle.c sphere.c triangle.c) \
		$(addprefix math/, math.c math_utils.c find_median.c dot_in_place_33.c dot_in_place_44.c rodrigues.c) \
)
OBJECTS := $(patsubst $(SOURCES_DIRECTORY)/%.c,$(OBJECTS_DIRECTORY)/%.o, $(SOURCES))

# test framework
CUNIT_DIRECTORY := $(ROOT_DIR)/lib/cunit
CUNIT := $(CUNIT_DIRECTORY)/lib/libcunit.a
CUNIT_LIB := -L$(CUNIT_DIRECTORY)/lib -lcunit
TEST_SOURCES := $(addprefix test/, \
		test_rodrigues.c test_find_median.c test_math.c test_dot_in_place_33.c test_dot_in_place_44.c \
		test_factories2.c test_factories.c test_checker.c test_runner.c \
		test_cylinder.c test_plane.c test_rectangle.c test_sphere.c test_factories4.c test_factories3.c \
		test_triangle.c \
)

TEST_HEADER := $(ROOT_DIR)/test/test.h
TEST_OBJECTS := $(patsubst %.c, $(OBJECTS_DIRECTORY)/%.o, $(TEST_SOURCES))
TEST_EXEC := test_runner

# libft
LIBFT_DIRECTORY := $(ROOT_DIR)/lib/libft
LIBFT := $(LIBFT_DIRECTORY)/libft.a

# mlx
MLX_DIRECTORY := $(ROOT_DIR)/lib/minilibx-linux
LIBMLX := $(MLX_DIRECTORY)/libmlx.a

LIB_FLAGS := -L$(LIBFT_DIRECTORY)
LIB_FLAGS += -L$(MLX_DIRECTORY)    
LIB_FLAGS += -lft
LIB_FLAGS += -lmlx_Linux
LIB_FLAGS += -lXext
LIB_FLAGS += -lX11
LIB_FLAGS += -lm

CC := cc

BASE_CFLAGS := -Wall -Wextra -Werror
BASE_CFLAGS += -Wuninitialized
BASE_CFLAGS += -g
BASE_CFLAGS += -pthread
BASE_CFLAGS += -mavx
BASE_CFLAGS += -Ofast
BASE_CFLAGS += -mavx
BASE_CFLAGS += -march=native
BASE_CFLAGS += -ffunction-sections
BASE_CFLAGS += -finline-functions
BASE_CFLAGS += -I$(HEADERS_DIRECTORY) -I$(LIBFT_DIRECTORY) -I$(MLX_DIRECTORY) -I$(CUNIT_DIRECTORY)/include
CFLAGS := $(BASE_CFLAGS)
OPT_CFLAGS := $(BASE_CFLAGS) -flto

ifeq ($(GITHUB_ACTIONS),true)
	CFLAGS += -Wno-stringop-overflow
endif

all: $(OBJECTS_DIRECTORY) $(LIBMLX) $(LIBFT) $(NAME)

YELLOW=\033[0;33m
GREEN=\033[0;32m
NC=\033[0m

$(LIBMLX):
	@if [ ! -d "$(MLX_DIRECTORY)" ]; then \
		mkdir -p $(MLX_DIRECTORY); \
		echo "$(YELLOW)Cloning MiniLibX repository...$(NC)"; \
		git clone https://github.com/42Paris/minilibx-linux.git $(MLX_DIRECTORY); \
	fi
	@echo "$(YELLOW)Compiling MiniLibX...$(NC)"
	@make -s -C $(MLX_DIRECTORY) >/dev/null 2>&1;
	@echo "$(GREEN)MiniLibX compiled$(NC)"

$(CUNIT):
	@if [ ! -d "$(CUNIT_DIRECTORY)" ]; then \
		echo "$(YELLOW)CUnit not found! Downloading and building...$(NC)"; \
		mkdir -p $(ROOT_DIR)/lib && cd $(ROOT_DIR)/lib && \
		wget https://sourceforge.net/projects/cunit/files/CUnit/2.1-3/CUnit-2.1-3.tar.bz2 && \
		tar -xvf CUnit-2.1-3.tar.bz2 && mv CUnit-2.1-3 cunit && rm CUnit-2.1-3.tar.bz2; \
	fi
	@echo "$(YELLOW)Building CUnit...$(NC)"
	@cd $(CUNIT_DIRECTORY) && autoreconf -fi && ./configure --prefix=$(CUNIT_DIRECTORY) && make && make install
	@echo "$(GREEN)CUnit compiled and installed$(NC)"

$(NAME): $(OBJECTS) $(LIBFT) $(LIBMLX)
	@$(CC) $(OPT_CFLAGS) $(OBJECTS) $(LIB_FLAGS) -o $(NAME) -Wl,--gc-sections

$(OBJECTS_DIRECTORY)/%.o: $(SOURCES_DIRECTORY)/%.c $(HEADERS) $(LIBMLX) | $(OBJECTS_DIRECTORY)
	@mkdir -p $(dir $@)
	@echo "Compiling $< with include path: $(MLX_DIRECTORY)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_DIRECTORY):
	@mkdir -p $(OBJECTS_DIRECTORY)

$(TEST_EXEC): $(filter-out $(OBJECTS_DIRECTORY)/main.o, $(OBJECTS)) $(TEST_OBJECTS) $(LIBFT) $(LIBMLX)
	@$(CC) $(TEST_OBJECTS) $(filter-out $(OBJECTS_DIRECTORY)/main.o, $(OBJECTS)) $(LIB_FLAGS) $(CUNIT_LIB) -o $(TEST_EXEC) -Wl,--gc-sections

$(OBJECTS_DIRECTORY)/test/%.o: test/%.c $(HEADERS) $(TEST_HEADER) | $(OBJECTS_DIRECTORY)/test
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_DIRECTORY)/test:
	@mkdir -p $(OBJECTS_DIRECTORY)/test

$(LIBFT):
	@echo "$(YELLOW)Compiling Libft...$(NC)"
	@make -C $(LIBFT_DIRECTORY) >/dev/null 2>&1;
	@echo "$(GREEN)Libft compiled$(NC)"

clean:
	@rm -rf $(OBJECTS_DIRECTORY)
	@rm -rf $(MLX_DIRECTORY) clean >/dev/null 2>&1
	@make -s -C $(LIBFT_DIRECTORY) fclean >/dev/null 2>&1
	@rm -rf $(CUNIT_DIRECTORY) clean >/dev/null 2>&1
	@rm -f $(TEST_EXEC)

fclean: clean
	@rm -f $(NAME)

re: fclean all

test: $(CUNIT) $(TEST_EXEC)
	@echo "$(GREEN)Running tests...$(NC)"
	@export LD_LIBRARY_PATH=$(CUNIT_DIRECTORY)/lib:$$LD_LIBRARY_PATH && ./$(TEST_EXEC)

.PHONY: all clean fclean re
