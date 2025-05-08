#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing/parsing.h"
# include <signal.h>
# include "../built_ins/built_ins.h"
# include <fcntl.h>

typedef struct s_data
{
	t_env	**envl;
	char	**env;
	int		read_fd;
}	t_data;


char	**get_path(char **env);
void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env);
int		check_built_in(char **args, t_env **env, int is_pipe);
void	exec_cmd(t_tree *node, char **env);
int		execute_cmd(t_tree *node, t_data *data, int is_pipe);
int		execute_or(t_tree *node, t_data *data, int is_pipe);
int		execute_and(t_tree *node, t_data *data, int is_pipe);
int		execute_pipe(t_tree *node, t_data *data);
int		execution(t_tree *node, char **env, t_env **envl, int is_pipe);
void	free_split(char **str);
int		execute_red_out(t_tree *node, t_data *data);

int		here_doc(t_tree *node, t_data *data);


#endif