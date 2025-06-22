/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_substitution.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:21:10 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/22 17:04:06 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_command_in_child(char *command, int pipe_fd[2], t_data *data)
{
	int		pid;
	t_list	*tokens;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork: ");
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		tokens = tokenize(command);
		if (tokens)
		{
			data->read_fd = STDIN_FILENO;
			data->cmd_sub = 1;
			build_execute(tokens, data);
		}
		exit(0);
	}
	return (pid);
}

char	*read_pipe_output(int fd)
{
	char	*output;
	char	buffer[1024];
	int		bytes_read;
	int		total_len;

	output = ft_strdup("");
	if (!output)
		return (NULL);
	total_len = 0;
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		output = ft_strjoin(output, buffer, 1);
		if (!output)
			return (perror("minishell: "), NULL);
		total_len += bytes_read;
		bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	}
	while (total_len > 0 && output[total_len - 1] == '\n')
	{
		output[total_len - 1] = '\0';
		total_len--;
	}
	return (output);
}

char	*expand_cmd_substitution(t_data *data, t_expand *expand, int *i)
{
	char	*line;
	char	*output;
	int		len;
	int		fd[2];
	int		pid;

	(*i) += 2;
	len = *i;
	while (expand->arg[len] && expand->arg[len] != ')')
		len++;
	line = ft_substr(expand->arg, *i, len - *i);
	if (!line)
		return (ft_strdup(""));
	(*i) += len - *i;
	if (pipe(fd) < 0)
		perror("minishell: ");
	pid = execute_command_in_child(line, fd, data);
	if (pid < 0)
		return (close(fd[0]), close(fd[1]), ft_strdup(""));
	close(fd[1]);
	waitpid(pid, NULL, 0);
	output = read_pipe_output(fd[0]);
	if (!output)
		return (ft_strdup(""));
	return (free(line), close(fd[0]), output);
}
