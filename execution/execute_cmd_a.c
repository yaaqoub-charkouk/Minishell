#include "execution.h"

char	**get_path(char **env)
{
	int		i;
	char	**path;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
	{
		perror("PATH");
		exit(EXIT_FAILURE);
	}
	path = ft_split(env[i] + 5, ':');
	if (!path)
	{
		perror("split");
		exit(EXIT_FAILURE);
	}
	return (path);
}

void	exec_cmd_from_path(char **path, char *cmd, char **args, char **env)
{
	char	*tmp;
	char	*tmp2;
	int		i;

	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		tmp2 = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(tmp2, X_OK) == 0)
		{
			execve(tmp2, args, env);
			free(tmp2);
			exit(EXIT_FAILURE);
		}
		if (tmp2)
			free(tmp2);
		i++;
	}
}

void	exec_cmd(t_tree *node, char **env)
{
	char	**path;

	if (ft_strchr(node->args[0], '/') && access(node->args[0], X_OK) == 0)
	{
		execve(node->args[0], node->args, env);
		perror("execve");
	}
	path = get_path(env);
	exec_cmd_from_path(path, node->args[0], node->args, env);
	free_split(path);
	write(2, "minishell: ", 11);
	write(2, node->args[0], ft_strlen(node->args[0]));
	write(2, ": command not found\n", 20);
	exit(127);
}

void    identify_read_write(t_tree *node)
{
	int	fd;
	if (node->red.outfile) // change the read fd
	{
		fd = open(node->red.outfile, node->red.flag, 0777);
		if (fd < 0)
		{
			perror("identify read write");
			close (fd);
		}
		dup2(fd, STDOUT_FILENO);
		
	}
	if (node->red.in_fd != -1) // change the read fd
	{
		dup2(node->red.in_fd, STDIN_FILENO);
		close(node->red.in_fd);
	}
}

int	execute_cmd(t_tree *node, t_data *data, int is_pipe)
{
	int		pid;
	int		status;

	if (!node || !node->args || !node->args[0])
		return (0);
	if (check_built_in(&node->args[0], data, is_pipe))
	{
		if (is_pipe)
			exit(0);
		return (0);
	}
	if (is_pipe == 1)
	{
		identify_read_write(node);
		exec_cmd(node, data->env);
		return (0);
	}
	else
	{
		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			identify_read_write(node);
			exec_cmd(node, data->env);
		}
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (1);
}
