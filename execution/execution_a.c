#include "execution.h"



int	check_built_in(char **args, t_env **env, int is_pipe)
{
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (built_in_cd(args, *env), 1);
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		return (built_in_echo(args, *env), 1);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		return (built_in_env(*env), 1);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		if (!is_pipe)
			printf("exit\n");
		return (built_in_exit(), 1);//exit need argument
	}
	else if (ft_strncmp(args[0], "export", 7) == 0)
		return (built_in_export(args, env), 1);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (built_in_pwd(), 1);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		return (built_in_unset(args, env), 1);
	return (0);
}

int	execute_and(t_tree *node, t_data *data, int is_pipe)
{
	// if (!node->left)
	// 	return (execution(node->right, data, is_pipe));
	if (execution(node->left, data, is_pipe) == 0)
		return (execution(node->right, data, is_pipe));
	return (1);
}

int	execute_or(t_tree *node, t_data *data, int is_pipe)
{
	if (execution(node->left, data, is_pipe) != 0)
		return (execution(node->right, data, is_pipe));
	return (0);
}

int	execution(t_tree *node, t_data *data, int is_pipe)
{
	if (!node)
		return (printf("no cmd to execute\n"),1);
	if (node->type == CMD)
		return (execute_cmd(node, data, is_pipe));
	if (node->type == PIPE)
		return (execute_pipe(node, data));
	if (node->type == OR)
		return (execute_or(node, data, is_pipe));
	if (node->type == AND)
		return (execute_and(node, data, is_pipe));
	return (1);
}

