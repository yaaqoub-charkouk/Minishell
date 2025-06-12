# ifndef PARSING_H
# define PARSING_H

# include "../libft/libft.h"
# include <stdio.h>
# include "../built_ins/built_ins.h"
# include <readline/history.h>
# include <readline/readline.h>
// linked list , basic , before the binary tree
extern int g_sig;
typedef struct s_redir t_redir;

typedef struct s_io_config
{
	char	*outfile;
	int		in_fd;
	int		flag;
	int		erno;
	char	*file_name;
} t_io_config;

typedef struct	s_tree
{
	char			*cmd;
	char			**args;
	t_type_node		type;
	t_io_config		red;
	struct s_tree	*right;
	struct s_tree	*left;
}	t_tree;

typedef struct	s_tokenize
{
	int			i;
	int			start;
	t_list		**head;
	t_list		**last;
	t_type_node	type;
}	t_tokenize;

typedef struct s_expand
{
	char	***args;
	char	*arg;
	char	**pile;
	int		*k;
	int		in_dquotes;
	int		in_squotes;
	int		*is_ambiguous;
} t_expand;


char	**ft_expand(char *cmd, char **cmd_args, t_data *data, int *is_ambiguous);

 /*======= shunting yard ========*/
t_list	*build_sy_queue(t_list	*token);

/**** tokenization ***/
t_type_node	get_type(char *content);
t_list		*tokenize(char	*line);
void	free_list(t_list *list);
void	free_matrix(char **args);


t_tree		*build_tree(t_list *tokens, t_data *data);
void		print_tokens(t_list *tokens);
int			is_syntax_error(char *line, t_list	*list);
t_tree		*new_tree_node(t_list	*token, t_data *data);

void		print_tree(t_tree *node, int level);

char		**ft_split_pipex(char const *s, char c);
char		**free_string(char **string, int i);

void	print_list(t_list	*list);

int		is_operator(t_type_node type); // from execution
int		is_redirection(t_type_node type);
int		count_words(const char *s, char sep);
char	**expand_wildcard(char *pattern);
void	expand_string(t_data *data, t_expand *expand);

#endif
