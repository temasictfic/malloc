# Check if HOSTTYPE is set, otherwise set it
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# Library names
NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC
LDFLAGS = -shared

# Include directories
INC_DIR = includes
LIBFT_DIR = libft

# Source files
SRC_DIR = src
SRC_FILES = malloc.c free.c realloc.c show_alloc_mem.c zone_management.c debug_malloc.c
SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

# Object files
OBJ_DIR = obj
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean fclean re libft test test_basic test_comprehensive

# Rules
all: $(NAME)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile libft
libft:
	$(MAKE) -C $(LIBFT_DIR)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Link the library
$(NAME): libft $(OBJS)
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft
	ln -sf $(NAME) $(LINK)

# Clean object files
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

# Clean object files and libraries
fclean: clean
	rm -f $(NAME) $(LINK)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

# Run all tests
test: test_basic test_comprehensive

# Run only basic tests
test_basic: $(NAME)
	@echo "\033[1;34mRunning basic tests...\033[0m"
	@sh tests/run_basic_test.sh

# Run only comprehensive tests
test_comprehensive: $(NAME)
	@echo "\033[1;34mRunning comprehensive tests...\033[0m"
	@sh tests/run_comprehensive_test.sh