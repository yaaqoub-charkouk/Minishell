CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
TARGET = minishell
SOURCES = main.c parsing_1_a.c parsing_2_y.c utils_1_y.c built_in_cmds_a.c build_tree_y.c

HEADERS = minishell.h
OBJECTS = $(SOURCES:.c=.o)
LIBFT_DIR = utils/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(TARGET)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(TARGET): $(OBJECTS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ -L$(LIBFT_DIR) -lreadline

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(TARGET)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
