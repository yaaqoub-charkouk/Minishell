#ifndef UTILS_H
#define UTILS_H
#include "../parsing/parsing.h"
char	**get_path(char **env, int *erno);
void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env);
void	check_access_err(t_tree *node, char **env);
void	exec_cmd(t_tree *node, char **env);
int		check_if_directory(t_tree *node);

#endif