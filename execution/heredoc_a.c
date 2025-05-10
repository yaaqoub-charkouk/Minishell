#include "execution.h"

int	execute_here_doc(t_tree *node, t_data *data)
{
	int		fd[2];
	char	*line;
	char	*limiter;
	size_t		limiter_len;


	limiter = node->args[0]; // may need to be protected;

	printf("limiter %s\n\n\n", limiter);
	limiter_len = ft_strlen(limiter);
	if (pipe(fd) == -1)
		return (perror("pipe"), 1);
	while (1)
	{
		line = readline(">");
		if (!line)
			exit (0);
		if (!ft_strncmp(line, limiter, limiter_len) 
			&& (ft_strlen(line)) == limiter_len)
			break ;
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line); // freeeeeee !!!!!!!!!!!
	}
	free(line);
	if (data->read_fd != STDIN_FILENO)
		close(data->read_fd);
	data->read_fd = fd[0];
	close(fd[1]);
	return (0);
}


static char	*open_heredocs(t_tree *node, t_data *data, t_list	**args_list, int	is_heredoc)
{
	if (!node)
		return (NULL);
	
	if (!node->right)
	{
		// delemiter is node->args[0];
		add_cmd_options(args_list, node->args, 1);
		if (is_heredoc)
			execute_here_doc(node, data);
		return (NULL);
	}

	if (node->right)
	{
		add_cmd_options(args_list, node->left->args, 1);
		// delemiter is node->left->args[0];

		if (is_heredoc)
			execute_here_doc(node->left, data);

		if (node->type != HEREDOC)
			return (NULL);
		// *red_type = node->type;
		return (open_heredocs(node->right, data, args_list, is_heredoc));
	}

	if (node->type == HEREDOC)
	{
		is_heredoc = 1;
		return (open_heredocs(node->right, data, args_list, is_heredoc));
	}

	return (NULL);
}


int	here_doc(t_tree *node, t_data *data)
{
	char	**arguments;
	t_list	*args_list;
	int		is_heredoc;

	is_heredoc = 0;
	args_list = NULL;
	arguments = NULL;

	if (node->type == HEREDOC)
		is_heredoc = 1;
	
	if (node->left)
	{
		arguments = node->left->args; // those line must be interpreted once ;
		args_list = add_cmd_options(&args_list, arguments, 0); // add left / first args ;
	}

	// if (node->right)
	if (!open_heredocs(node->right, data, &args_list, is_heredoc))
		return (1);

	arguments = list_to_char(args_list);
	node->args = arguments; // need to free old args

	execute_cmd(node, data, 0);

	return (0);
}



// int	here_doc(t_tree *node, t_data *data)
// {
// 	while (node)
// 	{
// 		if (node->type == HEREDOC)
// 		{
// 			if (execute_here_doc(node, data) != 0)
// 				return (1);
// 			else if (node->left)
// 				execute_cmd(node->left, data, 0);
// 		}
// 		node = node->left;
// 	}
// 	return (0);
// }

//