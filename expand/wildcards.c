/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:49:49 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/20 21:49:50 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	matches_pattern(char *pattern, const char *filename)
{
	if (!*pattern && !*filename)
		return (1);
	if (!*pattern && *filename)
		return (0);
	if (*pattern == '\033')
	{
		if (*filename == '*')
			return (matches_pattern(pattern + 1, filename + 1));
		return (0);
	}
	if (*pattern == '*')
	{
		if (matches_pattern(pattern + 1, filename))
			return (1);
		if (*filename && matches_pattern(pattern, filename + 1))
			return (1);
		return (0);
	}
	else if (*pattern == *filename)
		return (matches_pattern(pattern + 1, filename + 1));
	return (0);
}

void	sort_list(t_list **head)
{
	t_list	*curr;
	t_list	*next;
	char	*temp;

	if (!head || !*head)
		return ;
	curr = *head;
	while (curr && curr->next)
	{
		next = curr->next;
		while (next)
		{
			if (ft_strcmp(curr->content, next->content) > 0)
			{
				temp = curr->content;
				curr->content = next->content;
				next->content = temp;
			}
			next = next->next;
		}
		curr = curr->next;
	}
}

char	**expand_wildcard(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*wild;
	char			**result;

	wild = ((dir = opendir(".")), NULL);
	if (!dir)
		return (perror("minishell: opendir"), NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (matches_pattern(pattern, entry->d_name))
			ft_lstadd_back(&wild, ft_lstnew(ft_strdup(entry->d_name)));
		entry = readdir(dir);
	}
	closedir(dir);
	sort_list(&wild);
	result = list_to_char(wild);
	ft_lstclear(&wild, free);
	return (result);
}
