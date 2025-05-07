#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing/parsing.h"
# include "../built_ins/built_ins.h"
# include <fcntl.h>

int		execution(t_tree	*node, char **env, t_env **envl);
void	free_split(char **str);
int     execute_cmd(t_tree *node, char **env, t_env **envl);
int     execute_red_out(t_tree *node, char **env, t_env **envl);
int     check_built_in(char **args, t_env **env);
void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env);
char	**get_path(char **env);
#endif