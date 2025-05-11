#include "execution.h"

void	open_outfile(char	*filename, t_tree *entry_node, t_type_node type)
{
	int	flag;
	int	fd;

	printf("outfile : %s\n", filename);
	flag = O_CREAT | O_RDWR;
	if (type == REDIRECTION_OUT)
		flag |= O_TRUNC;
	else
		flag |= O_APPEND;
	fd = open(filename, flag, 0777);
	if (fd < 0)
	{
		perror("open_file");
		return ;
	}
	if (entry_node->fd[1] != -1)
		close(entry_node->fd[1]);
	entry_node->fd[1] = fd;
	printf("outfile fd : %d\n\n", fd);
}

void	open_infile(char *filename,	t_tree *entry_node)
{
	int	fd;

	printf("infile : %s\n", filename);
	fd = open(filename, O_RDONLY, 0777);
	if (fd < 0)	
	{
		perror("open_infile");
		return ;
	}
	if (entry_node->fd[0] != -1)
		close(entry_node->fd[0]);
	entry_node->fd[0] = fd;
	printf("infile fd : %d\n\n", fd);
}

void	open_heredoc(char	*limiter, t_tree *entry_node)
{
	int		fd[2];
	char	*line;
	size_t		limiter_len;

	printf("limiter %s\n\n\n", limiter);
	limiter_len = ft_strlen(limiter);
	if (pipe(fd) == -1)
	{
		perror("pipe"); 
		return ;
	}
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
		free(line); // freeeeeee  !!!!!!!!!!!
	}
	free(line);
	if (entry_node->fd[0] != -1)
		close(entry_node->fd[0]);
	entry_node->fd[0] = fd[0];
	close(fd[1]);
	printf("heredoc fd : %d", fd[0]);
}

void	open_fd(t_tree	*node, t_tree *entry_node, t_type_node *type, t_list **args_list)
{

	add_cmd_options(args_list, node->args, 1);

	if (*type == REDIRECTION_OUT || *type == APPEND)
	{
		open_outfile(node->args[0], entry_node, *type);
	}
	else if (*type == REDIRECTION_IN)
	{
		open_infile(node->args[0], entry_node);
	}
	else if (*type == HEREDOC)
	{
		open_heredoc(node->args[0], entry_node);
	}

	// decide how we should open the fd ;
}

// void	function(t_tree *entry_node, t_list **args_list, char **args)
// {
// 	if (entry_node->left) // assum that only the entry node args will be added 
// 	{
// 		add_cmd_options(&args_list, entry_node->left->args, 0);
// 	}
// }

void    traverse_branch(t_tree *node, t_tree *entry_node, t_type_node *type)
{
	t_list		*args_list;
	// static int	flag = 1;

	args_list = NULL;

	if (entry_node->left) // assum that only the entry node args will be added 
	{
		// flag = 0;
		add_cmd_options(&args_list, entry_node->left->args, 0);
	}
	
	
	if (node->left)
		open_fd(node->left, entry_node,  type, &args_list);
	else
		open_fd(node, entry_node, type, &args_list);

	
	*type = node->type;
	if (node->right)
		traverse_branch(node->right, entry_node, type);


	
	entry_node->args = list_to_char(args_list); // free old args;
	entry_node->type = CMD;

	// cmd->left = new_tree_node_args(list_to_char(args_list));
}


void	pre_execution(t_tree *node, t_data *data)
{
	t_type_node type;

	type = node->type;
	if (node->type == PIPE || node->type == OR || node->type == AND)
	{
		// call with left node
		pre_execution(node->left, data);
		// return ;
	}
	if (node->right)
	{
		traverse_branch(node->right, node, &type); // when we reach the start of a series of redirections;
		printf("may be something has changed \n");
	}

	// call with right
}
