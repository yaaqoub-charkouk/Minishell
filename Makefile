CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
TARGET = minishell

PARSING = $(addprefix parsing/, build_queue_y.c parsing_1_a.c tokenize_utils_y.c tokenize_y.c build_tree_y.c tree_utils_y.c)
BUILT_INS = $(addprefix built_ins/, echo.c exit.c pwd.c cd.c env.c export.c unset.c)
EXECUTION = $(addprefix execution/, execution_a.c utils_a.c)
SOURCES = main.c $(PARSING)  $(BUILT_INS) $(EXECUTION)

HEADERS = minishell.h
OBJECTS = $(SOURCES:.c=.o)
LIBFT_DIR = utils/libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(TARGET)
	make clean
	./minishell

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
