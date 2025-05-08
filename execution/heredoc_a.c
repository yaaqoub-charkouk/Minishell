#include "execution.h"

int	execute_here_doc(t_tree *node, t_data *data)
{
	int		fd[2];
	char	*line;
	char	*limiter;
	size_t		limiter_len;


	if (!node->right || !node->right->args[0])
	{
		if (!node->args || !node->args[0])
			return (perror("syntax error: no limiter"), 1);
		limiter = node->args[0];
	}
	else
		limiter = node->right->args[0];
	printf("limiter %s\n", limiter);
	limiter_len = ft_strlen(limiter);
	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	while (1)
	{
		line = readline(">");
		if (!line)
			break ;
		if (!ft_strncmp(line, limiter, limiter_len) 
			&& (ft_strlen(line)) == limiter_len)
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	free(line);
	if (data->read_fd != STDIN_FILENO)
		close(data->read_fd);
	data->read_fd = fd[0];
	close(fd[1]);
	return (0);
}

int	here_doc(t_tree *node, t_data *data)
{
	while (node)
	{
		if (node->type == HEREDOC)
		{
			if (execute_here_doc(node, data) != 0)
				return (1);
			else if (node->left)
				execute_cmd(node->left, data, 0);
		}
		node = node->left;
	}
	return (0);
}

//