#include "execution.h"

t_list	*add_cmd_options(t_list **args_list, char **args, int i)
{
	if (i == 0)
		*args_list = NULL;
	if (!args || !*args)
		return ( NULL); // delete printf;
	while (args[i])
	{
		ft_lstadd_back(args_list, ft_lstnew(ft_strdup(args[i]))); // because we fre args at expand ; 
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
		return ( NULL);
	i = 0;
	size = ft_lstsize(args_list);
	args_char = malloc((size + 1) * sizeof(char *));
	if(!args_char)
     return (NULL);
	while (i < size)
	{
		args_char[i] = ft_strdup(args_list->content); // no need to allocate;
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


	flag = O_CREAT | O_WRONLY;
	if (*(redir->type) == REDIRECTION_OUT)
		flag |= O_TRUNC;
	else
		flag |= O_APPEND;
	fd = open(filename, flag, 0777);
	if (fd < 0)
	{
		redir->open_error = errno;
		redir->entry_node->red.file_name = ft_strdup(filename);
		redir->entry_node->red.erno = redir->open_error;
		return ;
	}
	close(fd);
	free(redir->entry_node->red.outfile);
	redir->entry_node->red.outfile = ft_strdup(filename);
	redir->entry_node->red.flag = flag;
}

void	open_infile(char *filename,	t_redir	*redir)
{
	int	fd;

	fd = open(filename, O_RDONLY, 0777);
	if (fd < 0)	
	{
		redir->entry_node->red.file_name = ft_strdup(filename);
		redir->open_error = errno;
		redir->entry_node->red.erno = redir->open_error;
		return ;
	}
	if (redir->entry_node->red.in_fd != -1)
		close(redir->entry_node->red.in_fd);
	redir->entry_node->red.in_fd = fd;
}

void	open_heredoc(t_data *data, t_tree *node, t_redir *redir) // 
{
	int		fd[2];
	int		pid;
	char	*line;
	char	*limiter;
	char	*expanded_line;
	size_t	limiter_len;
	int		status;

	expanded_line = NULL;
	limiter = node->args[0];
	if (pipe(fd) == -1)
	{
		perror("pipe"); 
		return ;
	}
	if (ft_strchr(limiter, '\'') || ft_strchr(limiter, '\"'))
		redir->node->red.flag = 0;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		perror("heredoc");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		printf("limiter %s\n\n\n", limiter);
		limiter_len = ft_strlen(limiter);
		while (1)
		{
			line = readline(">");
			if (!line)
			exit (0);
			expanded_line = expand_heredoc(line, data, redir->node->red.flag);
			if (!ft_strncmp(line, limiter, limiter_len) 
				&& (ft_strlen(line)) == limiter_len)
				break ;
			// here should expand , afer check should expand 
			free(line);
			line = NULL;
			write(fd[1], expanded_line, ft_strlen(expanded_line));
			write(fd[1], "\n", 1);
			free(expanded_line);
			expanded_line = NULL;
		}
		free(line);
		free(expanded_line);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) // BEGIN:
		setup_signals();
	if (redir->entry_node->red.in_fd != -1)
		close(redir->entry_node->red.in_fd);
	redir->entry_node->red.in_fd = fd[0];
	close(fd[1]);
}

void	open_fd(t_data *data,t_tree	*node, t_redir *redir)
{
	int		k;
	int		is_ambiguous;
	char	*file_name; // NO LEAKS 👍🏻 // free it at execute cmd;

	file_name = ft_strdup(node->args[0]);
	is_ambiguous = 0;
	k = 0;
	add_cmd_options(&redir->args_list, node->args, 1);
	if (ft_strchr(node->args[0], '\"') || ft_strchr(node->args[0], '\''))
		node->red.flag = 0;
	if (*(redir->type) == HEREDOC)
	{
		ft_expand(NULL, node->args, data, NULL);
		open_heredoc(data, node, redir);
	}
	else
		node->args = ft_expand(NULL, node->args, data, &is_ambiguous);
	if (is_ambiguous)
	{
		redir->entry_node->red.erno = -1337;
		redir->entry_node->red.file_name = file_name;
		return ;
	}
	free(file_name);
	if (!redir->open_error && (*(redir->type) == REDIRECTION_OUT || *(redir->type) == APPEND))
		open_outfile(node->args[0], redir);
	else if (!redir->open_error && *(redir->type) == REDIRECTION_IN)
		open_infile(node->args[0], redir);
}


void	traverse_branch(t_data *data, t_tree *node, t_redir *redir)
{
	if (node->left)
		open_fd(data, node->left, redir);
	else 
		open_fd(data, node, redir);
	*(redir->type) = node->type;
	if (node->right)
		traverse_branch(data, node->right, redir);
}

int	bridge(t_data *data, t_tree *node, t_tree *entry_node, t_type_node *type)
{
	t_redir redir;

	redir.args_list = NULL;
	redir.node = node;
	redir.entry_node = entry_node;
	redir.type = type;
	redir.open_error = 0;

	if (redir.entry_node->left)
		add_cmd_options(&redir.args_list, redir.entry_node->left->args, 0);
	traverse_branch(data, node, &redir);
	if (is_redirection(redir.entry_node->type))
	{
		free_matrix(redir.entry_node->args);
		redir.entry_node->args = list_to_char(redir.args_list);
		ft_lstclear(&redir.args_list, free);
		redir.entry_node->type = CMD;
	}
	if (!entry_node->args && entry_node->red.in_fd != -1)// if there s no cmd expl << k close the heredoc read end we wont need it
	{
		close(entry_node->red.in_fd);
		entry_node->red.in_fd = -1;
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
		if (node->type == PIPE || node->type == OR || node->type == AND)
		{
			pre_execution(node->right, data);
		}
		if (node->right && (node->type == REDIRECTION_IN || node->type == REDIRECTION_OUT 
			|| node->type == APPEND || node->type == HEREDOC))
		{
			open_error = bridge(data, node->right, node, &type); // traverse a branch
		}
	}
	return (open_error);
}
