# ifndef PARSING_H
# define PARSING_H

# include "../utils/utils.h"

// linked list , basic , before the binary tree

char *expand_heredoc(char *line, t_data *data);
char	**ft_expand(char *cmd, char **cmd_args, t_data *data, int *is_ambiguous);
char	*accumulate_char(char *pile, char c);
char	*get_var_value(t_list *env, char *value, int *i, int *word_boundary);
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
