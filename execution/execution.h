#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing/parsing.h"
# include "../built_ins/built_ins.h"
# include <fcntl.h>

int		execution(t_tree	*node, char **env, t_env **envl);
void	free_split(char **str);
int     execute_cmd(t_tree *node, char **env, t_env **envl);
int     execute_red_out(t_tree *node, char **env, t_env **envl);
#endif