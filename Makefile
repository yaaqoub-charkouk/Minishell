CC = cc
CFLAGS =  -Wall -Wextra -Werror -I$(HOME) -g #-fsanitize=address
LDFLAGS = -L$(LIBFT_DIR) -L$(HOME)/readline -lreadline -lhistory 

TARGET = minishell

PARSING = $(addprefix parsing/, build_queue.c syntax.c tokenize.c build_tree.c syntax_utils.c syntax_utils2.c)
BUILT_INS = $(addprefix built_ins/, echo.c exit.c pwd.c cd.c env.c export.c unset.c env_utils.c export_utils.c)
EXECUTION = $(addprefix execution/, execution.c execute_cmd_utils.c redirections.c stream_redirect.c execute_cmd.c pipe.c)
SIGNALS = $(addprefix signals/, signal_1.c)
UTILS = $(addprefix utils/, utils.c free.c split_pipex.c redirections_utils.c parsing_utils.c)
EXPAND = $(addprefix expand/, wildcards.c expand.c expand_heredoc.c word_spliting.c expand_utils.c expand_variable.c ignore_empty_args.c command_substitution.c)

SOURCES = main.c main_utils.c $(PARSING) $(BUILT_INS) $(EXECUTION) $(SIGNALS) $(UTILS) $(EXPAND) 

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
