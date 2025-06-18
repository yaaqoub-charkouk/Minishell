#include "execution.h"

int	check_built_in(char **args, t_data *data, int is_pipe)
{
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (data->exit_status = built_in_cd(args, data), 1);
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		return (data->exit_status = built_in_echo(args), 1);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		return (data->exit_status = built_in_env(data), 1);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
	{
		if (!is_pipe)
			printf("exit\n");
		return (data->exit_status = built_in_exit(args, data), 1);//exit need argument
	}
	else if (ft_strncmp(args[0], "export", 7) == 0)
		return (data->exit_status = built_in_export(args, data), 1);
	else if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (data->exit_status = built_in_pwd(), 1);
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		return (data->exit_status = built_in_unset(args, data), 1);
	return (0);
}

int	execute_built_in(char **args, t_data *data, int is_pipe, t_tree *node)
{
	int	fd = -1;
	int	saved_stdout = -1;
	int	ret;

	if (node->red.outfile)
	{
		fd = open(node->red.outfile, node->red.flag, 0777);
		if (fd < 0)
			perror("minishell:");
		saved_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	ret = check_built_in(args, data, is_pipe);
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
		// printf("closed fd %d")
		// close(fd);
	}

	return (ret);
}

int	execute_and(t_tree *node, t_data *data, int is_pipe)
{
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
