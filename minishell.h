# ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
# include "./includes/libft.h"
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
typedef struct s_tree
{
	t_type_node	type;
	char			**cmd;
	struct s_tree	*left_node;
	struct s_tree	*right_node;
}	t_tree;

typedef struct s_list
{
	t_type_node			type;
	char			*content;
	struct s_list	*next;
}	t_list;


/**** tokenization ***/
t_type_node	get_type(char *content);

// test *****/
t_tree	*create_tree(t_list *list);
t_list	*new_node(char *content);
void	free_matrix(char **tokens);
// baghi nzid xi haja


#endif
