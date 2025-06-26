/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:58:16 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/26 16:44:27 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	open_heredoc_file(int *heredoc_n, int *fd_write, int *fd_read)
{
	char	*filename;

	filename = ft_strjoin("/tmp/heredoc_", ft_itoa((*heredoc_n)++), 2);
	unlink(filename);
	*fd_read = open(filename, O_CREAT | O_RDONLY | O_TRUNC, 0777);
	*fd_write = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	unlink(filename);
	free(filename);
	if (*fd_read < 0 || *fd_write < 0)
	{
		perror("minishell: ");
		return ;
	}
}

void	open_fd(t_data *data, t_tree	*node, t_redir *redir)
{
	int		k;
	int		is_ambiguous;
	char	*file_name;

	file_name = ((is_ambiguous = 0), (k = 0), ft_strdup(node->args[0]));
	add_cmd_options(&redir->args_list, node->args, 1);
	if (*(redir->type) == HEREDOC && !data->signaled)
		open_heredoc(data, node, redir);
	else
		node->args = ft_expand(NULL, node->args, data, &is_ambiguous);
	if (check_ambiguity(redir, file_name, is_ambiguous))
		return ;
	free(file_name);
	if (!node->args)
		return ;
	if (!redir->open_error && !data->signaled
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
