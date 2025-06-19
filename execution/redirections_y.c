/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_y.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:58:16 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/19 16:17:25 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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

void	open_heredoc(t_data *data, t_tree *node, t_redir *redir)
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
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		setup_signals();
		g_sig = 1;
		data->exit_status = 1;
	}
	if (redir->entry_node->red.in_fd != -1)
		close(redir->entry_node->red.in_fd);
	redir->entry_node->red.in_fd = fd[0];
	close(fd[1]);
}

void	open_fd(t_data *data, t_tree	*node, t_redir *redir)
{
	int		k;
	int		is_ambiguous;
	char	*file_name;

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
	if (check_ambiguity(redir, file_name, is_ambiguous))
		return ;
	free(file_name);
	if (!redir->open_error 
		&& (*(redir->type) == REDIRECTION_OUT || *(redir->type) == APPEND))
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
	t_redir	redir;

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
	if (!entry_node->args && entry_node->red.in_fd != -1)
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
		return (1);
	type = node->type;
	if (is_operator(node->type))
		pre_execution(node->left, data);
	if (node->right)
	{
		if (is_operator(node->type))
			pre_execution(node->right, data);
		if (node->right && is_redirection(node->type))
			open_error = bridge(data, node->right, node, &type);
	}
	return (open_error);
}
