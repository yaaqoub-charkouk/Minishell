# ifndef PARSING_H
# define PARSING_H

# include "../includes/libft.h"
# include <stdio.h>
# include "../built_ins/built_ins.h"
# include "readline/history.h"
# include "readline/readline.h"
// linked list , basic , before the binary tree
extern int g_sig;
typedef struct s_redir t_redir;

typedef struct s_redirection
{
	char	*outfile;
	int		in_fd;
	int		flag;
	int		erno;
	char	*file_name;
} t_redirection;

typedef struct	s_tree
{
	char			*cmd;
	char			**args;
	t_type_node		type;
	t_redirection	red;
	struct s_tree	*right;
	struct s_tree	*left;
}	t_tree;



/*==== stack to use for shunting yard algorithm ==== */

typedef struct	s_op // LIFO stack
{
	char			*content;
	t_type_node		type;
	struct	s_op	*next;
}	t_op;

typedef struct	s_queue // FIFO stack 
{
	char			*content;
	t_type_node		type;
	struct	s_queue *next;
}	t_queue;

 /*======= shunting yard ========*/
t_list	*build_sy_queue(t_list	*token);



/**** tokenization ***/
t_type_node	get_type(char *content);
t_list		*tokenize(char	*line);

t_op		*new_operator_node(t_list	*token);



t_tree		*build_tree(t_list *tokens, t_data *data);
void		print_tokens(t_list *tokens);
int			is_syntax_error(char *line, t_list	*list);
t_tree		*new_tree_node(t_list	*token, t_data *data);

void		print_tree(t_tree *node, int level);

char		**ft_split_pipex(char const *s, char c);
char		**free_string(char **string, int i);
char		**expand(char *cmd, t_data *data);

void	print_list(t_list	*list);

int		is_operator(t_type_node type); // from execution
int		is_redirection(t_type_node type);
int	count_words(const char *s, char sep);
char	**expand_wildcard(char *pattern);



#endif
