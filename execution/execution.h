#ifndef EXECUTION_H
# define EXECUTION_H
# include "../parsing/parsing.h"
# include <signal.h>
# include "../built_ins/built_ins.h"

int	execution(t_tree *node, char **env, t_env **envl, int is_pipe);
void	free_split(char **str);

#endif