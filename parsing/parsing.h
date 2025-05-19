# ifndef PARSING_H
# define PARSING_H

# include "../includes/libft.h"
# include <stdio.h>

# include "readline/history.h"
# include "readline/readline.h"
// linked list , basic , before the binary tree

typedef struct	s_tree
{
	char			*cmd;
	char			**args;
	t_type_node		type;
	int				fd[2]; // each cmd must have its own read and write fd;
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

/**** tokenization ***/
t_type_node	get_type(char *content);
t_list		*tokenize(char	*line);
t_queue  	*build_sy_queue(t_list  *tokens);
t_op		*new_operator_node(t_list	*token);
int			push_to_op_stack(t_op **stack_op, t_list	*token);
t_queue		*new_node_op_queue(t_op	*operator); // create a new queue for operator;
void		add_to_queue(t_queue **queue, t_queue *new); // add a t queue node to queue
t_queue		*new_node_queue(t_list	*lst);
t_tree		*build_tree(t_list *tokens);
void		print_tokens(t_list *tokens);
int			is_syntax_error(char *line, t_list	*list);
t_tree		*new_tree_node(t_list	*token);
t_tree		*new_tree_node_args(char	**arguments);
void		print_tree(t_tree *node, int level);
int			push(t_queue *queue, t_list **list);
char		**ft_split_pipex(char const *s, char c);
char	**free_string(char **string, int i);


void	print_list(t_list	*list);
int		is_redirection(t_type_node type);
#endif
