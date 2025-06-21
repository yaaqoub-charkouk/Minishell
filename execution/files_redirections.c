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

void	process_heredoc(t_data *data, t_redir *redir, char *limiter, int fd)
{
	size_t	limiter_len;
	char	*line;
	char	*expanded_line;

	signal(SIGINT, SIG_DFL);
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

void	check_heredoc_status(t_data *data, t_redir *redir, int status)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		setup_signals();
		g_sig = 1;
		data->exit_status = 1;
		data->signaled = 1;
	}
	if (redir->entry_node->red.in_fd != -1)
		close(redir->entry_node->red.in_fd);
}

void	open_heredoc(t_data *data, t_tree *node, t_redir *redir)
{
	int		fd[2];
	int		pid;
	int		status;

	if (!node || !node->args || !*node->args)
		return ;
	if (pipe(fd) == -1)
	{
		perror("pipe"); 
		return ;
	}
	if (ft_strchr(node->args[0], '\'') || ft_strchr(node->args[0], '\"'))
		redir->node->red.flag = 0;
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		perror("heredoc");
	if (pid == 0)
		process_heredoc(data, redir, node->args[0], fd[1]);
	waitpid(pid, &status, 0);
	check_heredoc_status(data, redir, status);
	redir->entry_node->red.in_fd = fd[0];
	close(fd[1]);
}
