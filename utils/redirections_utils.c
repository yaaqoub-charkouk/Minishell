/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:28:37 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/21 20:28:38 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_list	*add_cmd_options(t_list **args_list, char **args, int i)
{
	if (i == 0)
		*args_list = NULL;
	if (!args || !*args)
		return (NULL);
	while (args[i])
	{
		ft_lstadd_back(args_list, ft_lstnew(ft_strdup(args[i])));
		i++;
	}
	return (*args_list);
}

char	**list_to_char(t_list *args_list)
{
	int		size;
	int		i;
	char	**args_char;

	if (!args_list)
		return (NULL);
	i = 0;
	size = ft_lstsize(args_list);
	args_char = malloc((size + 1) * sizeof(char *));
	if (!args_char)
		return (NULL);
	while (i < size)
	{
		args_char[i] = ft_strdup(args_list->content);
		args_list = args_list->next;
		i++;
	}
	args_char[i] = NULL;
	return (args_char);
}

int	is_redirection(t_type_node type)
{
	return (type == REDIRECTION_IN || type == REDIRECTION_OUT 
		|| type == APPEND || type == HEREDOC);
}

int	get_operator_len(t_type_node type)
{
	if (type == APPEND || type == HEREDOC || type == AND || type == OR)
		return (2);
	return (1);
}

int	check_ambiguity(t_redir *redir, char *file_name, int is_ambiguous)
{
	if (is_ambiguous)
	{
		redir->entry_node->red.erno = -1337;
		redir->entry_node->red.file_name = file_name;
		return (1);
	}
	return (0);
}
