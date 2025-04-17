# ifndef MINISHEL_H
# define MINISHEL_H

#include <stdio.h>
#include <stdlib.h>
// # include "../includes/libft.h"
// # include "../includes/ft_printf.h"
#include <readline/readline.h>
#include <readline/history.h>


typedef enum s_type_node
{
	CMD,
	PIPE,
	REDIRECTION_IN,
	REDIRECTION_OUT,
	APPEND,
	HEREDOC,
	AND,
	OR,
} t_type_node;

// linked list , basic , before the binary tree
typedef struct s_list
{
	char			**cmd;
	struct s_list	*next;
}	t_list;

 typedef struct s_tree
 {
	char	**content;
	struct s_tree	*left_node;
	struct s_tree	*right_node;
 }	t_tree;

#endif
