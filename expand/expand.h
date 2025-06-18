#ifndef EXPAND_H
# define EXPAND_H

#include "../utils/utils.h"

char	**expand_wildcard(char *pattern);

char	*expand_heredoc(char *line, t_data *data, int	should_expand_var);
char	**ft_expand(char *cmd, char **cmd_args, t_data *data, int *is_ambiguous);
char	*get_var_value(t_list *env, char *value, int *i, int *word_boundary);
char	*accumulate_char(char *pile, char c);
#endif