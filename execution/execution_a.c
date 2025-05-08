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

void	exec_cmd(t_tree *node, char **env)
{
	char	**path;

	if (access(node->args[0], X_OK) == 0)
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

int	execute_cmd(t_tree *node, t_data *data, int is_pipe)
{
	int		pid;
	int		status;

	// if (!node || !node->args || !node->args[0])
	// 	return (1);
	if (check_built_in(&node->args[0], data->envl, is_pipe))
	{
		if (is_pipe)
			exit(0);
		return (0);
	}
	if (is_pipe == 1)
	{
		if (data->read_fd != STDIN_FILENO)
		{
			dup2(data->read_fd, STDIN_FILENO);
			close(data->read_fd);
		}
		exec_cmd(node, data->env);
	}
	else
	{
		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			if (data->read_fd != STDIN_FILENO)
			{
				dup2(data->read_fd, STDIN_FILENO);
				close(data->read_fd);
			}
			exec_cmd(node, data->env);
		}
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (1);
}

int	execute_and(t_tree *node, t_data *data, int is_pipe)
{
	if (execution(node->left, data->env, data->envl, is_pipe) == 0)
		return (execution(node->right, data->env, data->envl, is_pipe));
	return (1);
}

int	execute_or(t_tree *node, t_data *data, int is_pipe)
{
	if (execution(node->left, data->env, data->envl, is_pipe) != 0)
		return (execution(node->right, data->env, data->envl, is_pipe));
	return (0);
}

int	execute_pipe(t_tree *node, t_data *data)
{
	int	fd[2];
	int	pidl;
	int	pidr;
	int	status;

	if (pipe(fd) < 0)
		return (perror("pipe"), 1);
	pidl = fork();
	if (pidl < 0)
		return (close(fd[0]), close(fd[1]), perror("fork"), 1);
	if (pidl == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		exit(execution(node->left, data->env, data->envl, 1));
	}
	pidr = fork();
	if (pidr < 0)
		return (close(fd[0]), kill(pidl, SIGKILL), close(fd[1]), perror("fork"), 1); //we may need to kill the left process
	if (pidr == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		exit(execution(node->right, data->env, data->envl, 1));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pidl, &status, 0);
	waitpid(pidr, &status, 0);
	return (WEXITSTATUS(status));
}

int	execution(t_tree *node, char **env, t_env **envl, int is_pipe)
{
	t_data	data;

	data.envl = envl;
	data.env = env;
	data.read_fd = STDIN_FILENO;
	if (here_doc(node, &data))
		return (1);
	if (!node)
		return (1);
	if (node->type == CMD)
		return (execute_cmd(node, &data, is_pipe));
	if (node->type == PIPE)
		return (execute_pipe(node, &data));
	if (node->type == OR)
		return (execute_or(node, &data, is_pipe));
	if (node->type == AND)
		return (execute_and(node, &data, is_pipe));
	if (node->type == REDIRECTION_OUT || node->type == APPEND)
		return (execute_red_out(node, &data));
	return (1);
}
