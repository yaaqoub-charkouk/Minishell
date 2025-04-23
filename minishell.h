# ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
# include "includes/libft.h"
#include <readline/readline.h>
#include <readline/history.h>




// linked list , basic , before the binary tree
typedef struct s_tree
{
	char			*cmd;
	char			**args;
	struct s_tree	*left_node;
	struct s_tree	*right_node;
	t_type_node		type;
}	t_tree;




/**** tokenization ***/
t_type_node	get_type(char *content);
t_list		*tokenize(char	*line);

// test *****/
t_tree	*create_tree(t_list *list);
t_list	*new_node(char *content);
void	free_matrix(char **tokens);
// parsing adnan
int	is_syntax_error(char *line, t_list	*list);
#endif
