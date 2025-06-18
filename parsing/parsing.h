# ifndef PARSING_H
# define PARSING_H

# include "../utils/utils.h"

// linked list , basic , before the binary tree

 /*======= shunting yard ========*/
t_list	*build_sy_queue(t_list	*token);
void	add_token_to_queue(t_list **queue, t_list	*token);
/**** tokenization ***/
t_type_node	get_type(char *content);
t_list		*tokenize(char	*line);;
void	free_matrix(char **args);
void	handle_quotes(char c, int *in_squote, int *in_dquote);

t_tree		*build_tree(t_list *tokens, t_data *data);
void		print_tokens(t_list *tokens);
int			is_syntax_error(char *line, t_list	*list);
t_tree		*new_tree_node(t_list	*token, t_data *data);

void		print_tree(t_tree *node, int level);

char		**ft_split_pipex(char const *s, char c);
char		**free_string(char **string, int i);

void	print_list(t_list	*list);

int		count_words(const char *s, char sep);

#endif
