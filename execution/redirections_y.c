#include "execution.h"

t_list	*add_cmd_options(t_list **args_list, char **args, int i)
{
	if (i == 0)
		*args_list = NULL;
	if (!args || !*args)
		return (printf("segfault add cmd options\n"), NULL); // delete printf;
	while (args[i])
	{
		ft_lstadd_back(args_list, ft_lstnew(args[i]));
		i++;
	}
	return (*args_list);
}

char	**list_to_char(t_list  *args_list)
{
	int		size;
	int		i;
	char	**args_char;

	if (!args_list)
		return (printf("args_list are (null)\n"), NULL);
	i = 0;
	// print_list(args_list); // segfault , may be args_list was noot ended by null;
	size = ft_lstsize(args_list);
	args_char = malloc((size + 1) * sizeof(char *));
	if(!args_char)
     return (NULL);
	while (i < size)
	{
		args_char[i] = ft_strdup(args_list->content);
		args_list = args_list->next;
		i++;
	}
	args_char[i] = NULL;
	return (args_char); // need to be freed ;
}

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
	printf("heredoc fd : %d\n", fd[0]);
}

void	open_fd(t_tree	*node, t_tree *entry_node, t_type_node *type, t_list **args_list)
{

	add_cmd_options(args_list, node->args, 1);
	printf("\n\nargs of %s added %s\n\n", node->args[0], node->args[1]);

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


void    traverse_branch(t_tree *node, t_tree *entry_node, t_type_node *type, t_list	**args_list)
{


	if (node->left)
		open_fd(node->left, entry_node,  type, args_list);
	else 
		open_fd(node, entry_node, type, args_list);

	*type = node->type;
	if (node->right)
		traverse_branch(node->right, entry_node, type, args_list); // should be returned;
	
	// end of a series of recursion 





	// the code should be called only at the end of a branch;
	
}

void	bridge(t_tree *node, t_tree *entry_node, t_type_node *type)
{
	t_list *args_list;

	args_list = NULL;
	if (entry_node->left)
	{
		add_cmd_options(&args_list, entry_node->left->args, 0);
		printf("initial args %s added \n", entry_node->left->args[0]);
	}


	traverse_branch(node, entry_node, type, &args_list);

	// end of recursion

	if (entry_node->type == REDIRECTION_IN || entry_node->type == REDIRECTION_OUT 
			|| entry_node->type == APPEND || entry_node->type == HEREDOC)
	{
		printf("\nentry node type %d\n", entry_node->type);
		
		entry_node->args = list_to_char(args_list); // NULL check
		printf("cmd to execute ");
		print_list(args_list);
		entry_node->type = CMD;
		printf("in fd %d\n", entry_node->fd[0]);
		printf("out fd %d\n", entry_node->fd[1]);
		printf("\nentry node type %d\n", entry_node->type);
		printf("\nentry node %s has been modified to : \n", entry_node->cmd);
	}
}


void	pre_execution(t_tree *node, t_data *data)
{
	t_type_node	type;

	if (!node)
	{
		printf("tree node is null segfault\n");
		return ;
	}
	type = node->type;
	if (node->type == PIPE || node->type == OR || node->type == AND)
	{
		// call with left node
		pre_execution(node->left, data);
		// return ;
	}
	if (node->right)
	{
		printf("entry node : %s \n", node->cmd);
		if (node->type == PIPE || node->type == OR || node->type == AND)
		{
			pre_execution(node->right, data);
			// node = node->right;
		}

		
		if (node->right && (node->type == REDIRECTION_IN || node->type == REDIRECTION_OUT 
			|| node->type == APPEND || node->type == HEREDOC))
		{
			bridge(node->right, node, &type); // when we reach the start of a series of redirections;
			// pre_execution(node->left, data);
		}
	}

	// call with right
}
