# ifndef MINISHELL_H
# define MINISHELL_H

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



/**** tokenization ***/
t_type_node	get_type(char *content);
t_list		*tokenize(char	*line);

// test *****/
// t_tree	*create_tree(t_list *list);

t_queue  *build_sy_queue(t_list  *tokens);

// parsing adnan
int	is_syntax_error(char *line, t_list	*list);
#endif
