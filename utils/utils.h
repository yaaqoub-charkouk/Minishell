#ifndef UTILS_H
# define UTILS_H

# include "struct.h"
# include "../signals/signals.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>
# include <sys/wait.h>

char	**get_path(char **env, int *erno);
void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env);
void	check_access_err(t_tree *node, char **env);
void	exec_cmd(t_tree *node, char **env);
int		check_if_directory(char	*cmd);

//utils.c
void	close_read_fd(t_tree *node);
void	free_split(char **str);
int		ft_strcmp(const char *s1, const char *s2);

// free
void	free_tree(t_tree *tree);
void	free_matrix(char **args);
char	**ft_split_pipex(const char *s, char c);

// redirections utils
char	**list_to_char(t_list  *args_list);
t_list	*add_cmd_options(t_list **args_list, char **args, int i);
int     is_redirection(t_type_node type);
int     is_operator(t_type_node type);
int     get_operator_len(t_type_node type);

#endif