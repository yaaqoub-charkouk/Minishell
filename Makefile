CC = cc
CFLAGS =  -Wall -Wextra -Werror -I$(HOME) -g #-fsanitize=address
LDFLAGS = -L$(LIBFT_DIR) -L$(HOME)/readline -lreadline -lhistory 

TARGET = minishell


PARSING = $(addprefix parsing/, build_queue_y.c parsing_1_a.c tokenize_y.c build_tree_y.c)
BUILT_INS = $(addprefix built_ins/, echo.c exit.c pwd.c cd.c env.c export.c unset.c env_utils.c export_utils.c)
EXECUTION = $(addprefix execution/, execution_a.c redirections_y.c execute_cmd_a.c pipe_a.c)
SIGNALS = $(addprefix signals/, signal_1_y.c)
UTILS = $(addprefix utils/, execute_cmd_utils.c utils.c free.c split_pipex.c redirections_utils.c)
EXPAND = $(addprefix expand/, wildcards.c expand.c expand_heredoc.c)

SOURCES = main.c $(PARSING) $(BUILT_INS) $(EXECUTION) $(SIGNALS) $(UTILS) $(EXPAND)

READLINE = 
HEADERS = minishell.h
OBJECTS = $(SOURCES:.c=.o)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(TARGET) clean

$(LIBFT):
	make -C $(LIBFT_DIR)

$(TARGET): $(OBJECTS) $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

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
