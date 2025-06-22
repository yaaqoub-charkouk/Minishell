CC = cc
CFLAGS =  -Wall -Wextra -Werror -I$(HOME)
LDFLAGS = -L$(LIBFT_DIR) -L$(HOME)/readline -lreadline -lhistory 

TARGET = minishell
TARGET_BONUS = minishell_bonus

HEADERS = minishell expand/expand parsing/parsing utils/utils \
		built_ins/built_ins signals/signals execution/execution utils/struct
HEADERS_MANDATORY = $(addsuffix .h, $(HEADERS))
HEADERS_BONUS = $(addprefix bonus/, $(addsuffix _bonus.h, $(HEADERS)))

PARSING = $(addprefix parsing/, build_queue syntax tokenize build_tree syntax_utils syntax_utils2)
BUILT_INS = $(addprefix built_ins/, echo exit pwd cd env export unset env_utils export_utils)
EXECUTION = $(addprefix execution/, execution execute_cmd_utils redirections stream_redirect execute_cmd pipe)
SIGNALS = $(addprefix signals/, signal_1)
UTILS = $(addprefix utils/, utils free split_pipex redirections_utils parsing_utils)
EXPAND = $(addprefix expand/, wildcards expand expand_heredoc word_spliting expand_utils expand_variable ignore_empty_args command_substitution)

SRC = main main_utils $(PARSING) $(BUILT_INS) $(EXECUTION) $(SIGNALS) $(UTILS) $(EXPAND)
SOURCES = $(addsuffix .c, $(SRC))
SRC_BONUS = $(addprefix bonus/, $(addsuffix _bonus.c, $(SRC)))

OBJECTS = $(SOURCES:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_HEADER = libft/libft.h

all: $(LIBFT) $(TARGET) 

bonus: $(LIBFT) $(TARGET_BONUS)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(TARGET): $(OBJECTS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET_BONUS): $(OBJ_BONUS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bonus/%.o: bonus/%.c $(HEADERS_BONUS) $(LIBFT_HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c  $(HEADERS_MANDATORY) $(LIBFT_HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(OBJ_BONUS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(TARGET)
	rm -f $(TARGET_BONUS)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
