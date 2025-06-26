/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream_redirect_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:25:58 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/26 16:46:24 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution_bonus.h"

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
		redir->entry_node->red.erno = errno;
		return ;
	}
	if (redir->entry_node->red.in_fd != -1)
		close(redir->entry_node->red.in_fd);
	redir->entry_node->red.in_fd = fd;
}

void	process_heredoc(t_data *data, t_tree *node, int fd)
{
	size_t	limiter_len;
	char	*line;
	char	*expanded_line;

	limiter_len = ft_strlen(node->args[0]);
	cmd_sub_heredoc(data->cmd_sub, fd);
	while (1)
	{
		line = readline(">");
		if (!line)
			exit (0);
		expanded_line = expand_heredoc(line, data, node->red.flag);
		if (!ft_strncmp(line, node->args[0], limiter_len) 
			&& (ft_strlen(line)) == limiter_len)
			break ;
		free(line);
		line = NULL;
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(expanded_line);
		expanded_line = NULL;
	}
	free(line);
	free(expanded_line);
	exit(EXIT_SUCCESS);
}

void	check_heredoc_status(t_data *data, t_redir *redir, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		setup_signals();
		g_sig = 1;
		data->exit_status = 1;
		data->signaled = 1;
		printf(">\n");
	}
	if (redir->entry_node->red.in_fd != -1)
	{
		close(redir->entry_node->red.in_fd);
		redir->entry_node->red.in_fd = -1;
	}
}

void	open_heredoc(t_data *data, t_tree *node, t_redir *redir)
{
	int			fd_read;
	int			fd_write;
	int			pid;
	static int	heredoc_n;

	if (!node || !node->args || !*node->args)
		return ;
	if (ft_strchr(node->args[0], '\'') || ft_strchr(node->args[0], '\"'))
		node->red.flag = 0;
	else
		node->red.flag = 1;
	ft_expand(NULL, node->args, data, NULL);
	open_heredoc_file(&heredoc_n, &fd_write, &fd_read);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		perror("minishell: ");
	if (pid == 0)
		process_heredoc(data, node, fd_write);
	check_heredoc_status(data, redir, pid);
	redir->entry_node->red.in_fd = fd_read;
	close(fd_write);
}
