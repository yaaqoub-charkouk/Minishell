/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_utils_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:49:22 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 17:39:29 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution_bonus.h"

char	**get_path(char **env, int *erno)
{
	int		i;
	char	**path;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
	{
		*erno = ENOENT;
		return (NULL);
	}
	path = ft_split(env[i] + 5, ':');
	if (!path)
	{
		perror("minishell: ");
		exit(EXIT_FAILURE);
	}
	return (path);
}

void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env)
{
	char	*tmp;
	char	*tmp2;
	int		i;

	if (cmd && cmd[0] == '\0')
		return ;
	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/", 0);
		tmp2 = ft_strjoin(tmp, cmd, 1);
		if (access(tmp2, X_OK) == 0)
		{
			if (check_if_directory(tmp2, cmd, 2))
				exit(127);
			execve(tmp2, args, env);
			free(tmp2);
			exit(EXIT_FAILURE);
		}
		if (tmp2)
			free(tmp2);
		i++;
	}
}

void	check_access_err(t_tree *node, char **env)
{
	if (access(node->args[0], X_OK) == 0) 
	{
		execve(node->args[0], node->args, env);
		perror("minishell: ");
		exit(1);
	}
	if (node->red.erno)
	{
		ft_putstr_fd("minishell :", 2);
		ft_putstr_fd(strerror(node->red.erno), 2);
		write(2, "\n", 1);
		exit(127);
	}
	write(2, "minishell: ", 11);
	write(2, node->args[0], ft_strlen(node->args[0]));
	write(2, ": command not found\n", 20);
	exit(127);
}

void	exec_cmd(t_tree *node, char **env)
{
	char	**path;

	if (ft_strchr(node->args[0], '/'))
	{
		if (check_if_directory(node->args[0], NULL, 1))
			exit(126);
		execve(node->args[0], node->args, env);
		perror("minishell: ");
		if (errno == ENOENT)
			exit(127);
		else if (errno == EACCES || errno == ENOEXEC)
			exit(126);
		else
			exit(1);
	}
	path = get_path(env, &node->red.erno);
	if (path)
	{
		exec_cmd_from_path(path, node->args[0], node->args, env);
		free_matrix(path);
	}
	check_access_err(node, env);
	close_read_fd(node);
}

int	check_if_directory(char	*cmd, char *direct, int should_print)
{
	DIR	*dir;

	dir = opendir(cmd);
	if (dir)
	{
		if (should_print == 1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			write(2, ": ", 2);
			ft_putstr_fd(strerror(EISDIR), 2);
			write(2, "\n", 1);
		}
		if (should_print == 2)
		{
			write(2, "minishell: ", 11);
			write(2, direct, ft_strlen(direct));
			write(2, ": command not found\n", 20);
		}
		closedir(dir);
		return (1);
	}
	return (0);
}
