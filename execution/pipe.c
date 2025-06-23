/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:49:30 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/23 13:09:37 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	execute_left(t_tree *node, t_data *data, int *fd)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	if (node->left->type == CMD)
		exit(execution(node->left, data, 1));
	else
		exit(execution(node->left, data, 0));
}

void	execute_right(t_tree *node, t_data *data, int *fd)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	if (node->right->type == CMD)
		exit(execution(node->right, data, 1));
	else
		exit(execution(node->right, data, 0));
}

int	exit_status(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_pipe(t_tree *node, t_data *data)
{
	int	fd[2];
	int	pidl;
	int	pidr;
	int	status;

	if (pipe(fd) < 0)
		return (perror("minishell: "), 1);
	signal(SIGINT, SIG_IGN);
	pidl = fork();
	if (pidl < 0)
		return (close(fd[0]), close(fd[1]), perror("minishell: fork: "), 1);
	if (pidl == 0)
		execute_left(node, data, fd);
	pidr = fork();
	if (pidr < 0)
		return (close(fd[0]), kill(pidl, SIGKILL),
			close(fd[1]), perror("minishell: fork: "), 1);
	if (pidr == 0)
		execute_right(node, data, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pidl, &status, 0);
	waitpid(pidr, &status, 0);
	return (exit_status(status));
}
