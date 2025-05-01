# ifndef MINISHELL_H
# define MINISHELL_H

#include "built_ins/built_ins.h"
#include <stdio.h>
#include <stdlib.h>
# include "includes/libft.h"
#include <readline/readline.h>
#include <readline/history.h>




// linked list , basic , before the binary tree

typedef struct	s_tree
{
	char			*cmd;
	char			**tokens;
	struct s_tree	*left_node;
	struct s_tree	*right_node;
	t_type_node		type;
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

// typedef struct s_env
// {
// 	char			*key;
// 	char			*value;
// 	struct s_env	*next;
// } t_env;


/**** tokenization ***/
t_type_node	get_type(char *content);
t_list		*tokenize(char	*line);

// test *****/
// t_tree	*create_tree(t_list *list);

t_queue  	*build_sy_queue(t_list  *tokens);
t_op		*new_operator_node(t_list	*token);
int			push_to_op_stack(t_op **stack_op, t_list	*token);

t_queue		*new_node_op_queue(t_op	*operator); // create a new queue for operator;
void		add_to_queue(t_queue **queue, t_queue *new); // add a t queue node to queue
t_queue		*new_node_queue(t_list	*lst);
t_tree  *build_tree(t_queue *queue);

// parsing adnan
int	is_syntax_error(char *line, t_list	*list);
#endif
