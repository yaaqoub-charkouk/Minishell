#include "execution.h"

void    identify_read_write(t_tree *node)
{
	int	fd;
	if (node->red.outfile) // change the read fd
	{
		fd = open(node->red.outfile, node->red.flag, 0777);
		if (fd < 0)
		{
			perror("identify read write");
			return ;
		}
		free(node->red.outfile);
		dup2(fd, STDOUT_FILENO);
		close (fd);
	}
	if (node->red.in_fd != -1) // change the read fd
	{
		dup2(node->red.in_fd, STDIN_FILENO);
		close(node->red.in_fd);
	}
}

int	handle_redirection_err(t_tree *node, int is_pipe)
{
	if (node->red.erno)
	{
		ft_putstr_fd("minishell :" ,2);
		ft_putstr_fd(node->red.file_name,2);
		write(2, ": " , 2);
		if (node->red.erno != -1337)
			ft_putstr_fd(strerror(node->red.erno), 2);
		else
			ft_putstr_fd("ambiguous redirect", 2);
		write(2, "\n", 1);
		// free(node->red.outfile);
		free(node->red.file_name);
		node->red.file_name = NULL;
		if (is_pipe)
			exit(1);
		return (1);
	}
	return (0);
}

int	execute_fork_command(t_tree *node, t_data *data)
{
	int	pid;
	int	status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		identify_read_write(node);
		if (check_if_directory(node))
			exit(126);
		else
			exec_cmd(node, data->env);
	}
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_pipe_cmd(t_tree *node, t_data *data)
{
	identify_read_write(node);
	if (check_if_directory(node))
		return (126);
	else
		exec_cmd(node, data->env);
	return (0);
}

int	execute_cmd(t_tree *node, t_data *data, int is_pipe)
{
	int		temp;

	if (!node || !node->args || !node->args[0])
	{
		if (node && node->red.erno)
			return (handle_redirection_err(node, is_pipe)); // we should free in case of failure at open ;
		return (0);
	}
	if (node->red.erno)
		return (handle_redirection_err(node, is_pipe));
	node->args = ft_expand(NULL, node->args, data, &temp);
	if (check_built_in(&node->args[0], data, is_pipe))
	{
		if (is_pipe)
			exit(1);
		return (data->exit_status);
	}
	if (is_pipe == 1)
		execute_pipe_cmd(node, data);
	else
		return (execute_fork_command(node, data));
		// if (node->red.in_fd != -1) // closing the read fd in parent
		// {
		// 	close(node->red.in_fd);
		// 	node->red.in_fd = -1;
		// }
	return (1);
}
