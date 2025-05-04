CC = gcc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address
TARGET = minishell

PARSING = $(addprefix parsing/, build_queue_y.c parsing_1_a.c utils_1_y.c build_tree_y.c tokenize_y.c)
BUILT_INS = $(addprefix built_ins/, echo.c exit.c pwd.c cd.c env.c export.c unset.c)
# EXECUTION = &(addprefix execution/, )
SOURCES = main.c $(PARSING)  $(BUILT_INS)

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
