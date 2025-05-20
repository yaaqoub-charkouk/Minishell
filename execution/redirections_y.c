#include "execution.h"

t_list	*add_cmd_options(t_list **args_list, char **args, int i)
{
	if (i == 0)
		*args_list = NULL;
	if (!args || !*args)
		return (printf("add cmd options\n"), NULL); // delete printf;
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
		return (printf("list to char\n"), NULL);
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

void	open_outfile(char	*filename, t_redir *redir)
{
	int	flag;
	int	fd;

	printf("outfile : %s\n", filename);
	flag = O_CREAT | O_RDWR;
	if (*(redir->type) == REDIRECTION_OUT)
		flag |= O_TRUNC;
	else
		flag |= O_APPEND;
	fd = open(filename, flag, 0777);
	if (fd < 0)
	{
		perror("open_file");
		redir->open_error = 1;
		return ;
	}
	close(fd);
	redir->entry_node->red.outfile = filename;
	redir->entry_node->red.flag = flag;
}

void	open_infile(char *filename,	t_redir	*redir)
{
	int	fd;

	printf("infile : %s\n", filename);
	fd = open(filename, O_RDONLY, 0777);
	if (fd < 0)	
	{
		perror("open_infile");
		redir->open_error = 1;
		return ;
	}
	if (redir->entry_node->red.in_fd != -1)
		close(redir->entry_node->red.in_fd);
	redir->entry_node->red.in_fd = fd;
}

void	open_heredoc(char	*limiter, t_redir *redir)
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
	if (redir->entry_node->red.in_fd != -1)
		close(redir->entry_node->red.in_fd);
	redir->entry_node->red.in_fd = fd[0];
	close(fd[1]);
}

void	open_fd(t_tree	*node, t_redir *redir)
{
	add_cmd_options(&redir->args_list, node->args, 1);
	// printf("\n\nargs of %s added %s\n\n", node->args[0], node->args[1]);

	if (!redir->open_error && (*(redir->type) == REDIRECTION_OUT || *(redir->type) == APPEND))
	{
		open_outfile(node->args[0], redir);
	}
	else if (!redir->open_error && *(redir->type) == REDIRECTION_IN)
	{
		open_infile(node->args[0], redir);
	}
	else if (*(redir->type) == HEREDOC)
	{
		open_heredoc(node->args[0], redir);
	}
}


void    traverse_branch(t_tree *node, t_redir *redir)
{
	if (node->left)
		open_fd(node->left, redir); // open the outfile
	else 
		open_fd(node, redir); // set outfile 

	*(redir->type) = node->type;
	if (node->right)
		traverse_branch(node->right, redir);
}

int	bridge(t_tree *node, t_tree *entry_node, t_type_node *type)
{
	t_redir redir;

	redir.args_list = NULL;
	redir.node = node;
	redir.entry_node = entry_node;
	redir.type = type;
	redir.open_error = 0;

	if (redir.entry_node->left)
	{
		add_cmd_options(&redir.args_list, redir.entry_node->left->args, 0);
		// printf("initial args %s added \n", redir.entry_node->left->args[0]);
	}
	traverse_branch(node, &redir);

	// end of recursion

	if (redir.entry_node->type == REDIRECTION_IN || redir.entry_node->type == REDIRECTION_OUT 
			|| redir.entry_node->type == APPEND || redir.entry_node->type == HEREDOC)
	{
		
		redir.entry_node->args = list_to_char(redir.args_list); // NULL check
		redir.entry_node->type = CMD;

		// print statements
		printf("\nentry node %s has been modified to : \n", redir.entry_node->cmd);
		printf("cmd to execute ");
		print_list(redir.args_list);
		// printf("in fd %d\n", redir.entry_node->fd[0]);
		// printf("out fd %d\n", redir.entry_node->fd[1]);
	}
	return (redir.open_error);
}


int	pre_execution(t_tree *node, t_data *data)
{
	t_type_node	type;
	int			open_error;

	open_error = 0;
	if (!node)
	{
		printf("tree node is null segfault\n");
		return (1);
	}
	type = node->type;
	if (node->type == PIPE || node->type == OR || node->type == AND)
	{
		pre_execution(node->left, data);
	}
	if (node->right)
	{
		printf("entry node : %s \n", node->cmd);
		if (node->type == PIPE || node->type == OR || node->type == AND)
		{
			pre_execution(node->right, data);
		}
		if (node->right && (node->type == REDIRECTION_IN || node->type == REDIRECTION_OUT 
			|| node->type == APPEND || node->type == HEREDOC))
		{
			open_error = bridge(node->right, node, &type);
		}
	}
	return (open_error);
}
