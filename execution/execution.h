#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing/parsing.h"
# include <signal.h>
# include "../built_ins/built_ins.h"
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>
# include <sys/wait.h>

typedef struct s_redir
{
	t_tree		*node;
	t_tree		*entry_node;
	t_type_node	*type;
	t_list		*args_list;
	int			open_error;
}	t_redir;

char	**get_path(char **env, int *erno);
void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env);
int		check_built_in(char **args, t_data *data, int is_pipe);
void	exec_cmd(t_tree *node, char **env);
int		execute_cmd(t_tree *node, t_data *data, int is_pipe);
int		execute_or(t_tree *node, t_data *data, int is_pipe);
int		execute_and(t_tree *node, t_data *data, int is_pipe);
int		execute_pipe(t_tree *node, t_data *data);
int		execution(t_tree *node, t_data *data, int is_pipe);
void	free_split(char **str);
t_list	*add_cmd_options(t_list **args_list, char **args, int i);
char	**list_to_char(t_list  *env);
int		pre_execution(t_tree *node, t_data *data);
int		is_redirection(t_type_node type);
#endif