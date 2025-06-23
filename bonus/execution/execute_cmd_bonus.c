/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:49:19 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/23 13:00:06 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution_bonus.h"

void	identify_read_write(t_tree *node)
{
	int	fd;

	if (node->red.outfile)
	{
		fd = open(node->red.outfile, node->red.flag, 0777);
		if (fd < 0)
		{
			perror("minishell: ");
			return ;
		}
		free(node->red.outfile);
		dup2(fd, STDOUT_FILENO);
		close (fd);
	}
	if (node->red.in_fd != -1)
	{
		dup2(node->red.in_fd, STDIN_FILENO);
		close(node->red.in_fd);
	}
}

int	handle_redirection_err(t_tree *node, int is_pipe)
{
	if (node && node->red.erno)
	{
		ft_putstr_fd("minishell :", 2);
		ft_putstr_fd(node->red.file_name, 2);
		write(2, ": ", 2);
		if (node->red.erno != -1337)
			ft_putstr_fd(strerror(node->red.erno), 2);
		else
			ft_putstr_fd("ambiguous redirect", 2);
		write(2, "\n", 1);
		free(node->red.file_name);
		node->red.file_name = NULL;
		if (is_pipe)
			exit(1);
		return (1);
	}
	return (g_sig);
}

int	execute_fork_command(t_tree *node, t_data *data)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid < 0)
		return (perror("minishell: "), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		identify_read_write(node);
		exec_cmd(node, data->env);
	}
	waitpid(pid, &status, 0);
	return (exit_status(status));
}

int	execute_pipe_cmd(t_tree *node, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	identify_read_write(node);
	exec_cmd(node, data->env);
	return (0);
}

int	execute_cmd(t_tree *node, t_data *data, int is_pipe)
{
	int		temp;

	if (!node || !node->args || !node->args[0])
		return (handle_redirection_err(node, is_pipe));
	if (data->signaled)
		return (signal_exit(node, is_pipe));
	if (node->red.erno)
		return (handle_redirection_err(node, is_pipe));
	node->args = ft_expand(NULL, node->args, data, &temp);
	if (!node->args)
		return (data->exit_status = 0, 0);
	if (execute_built_in(&node->args[0], data, is_pipe, node))
	{
		if (is_pipe)
			exit(data->exit_status);
		return (data->exit_status);
	}
	if (is_pipe == 1)
		return (execute_pipe_cmd(node, data));
	else
		return (execute_fork_command(node, data));
	return (1);
}
