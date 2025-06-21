/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:50:31 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/20 21:50:32 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	close_read_fd(t_tree *node)
{
	if (node->red.in_fd != -1)
	{
		close(node->red.in_fd);
		node->red.in_fd = -1;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (*str1)
	{
		if (*str1 != *str2)
			return (*str1 - *str2);
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}

int	check_sep(char c, char sep)
{
	return (c == sep);
}

char	**free_strs(char **strs, int i)
{
	while (i >= 0)
		free(strs[i--]);
	free(strs);
	return (NULL);
}

int	signal_exit(t_tree *node, int is_pipe)
{
	close_read_fd(node);
	if (is_pipe)
		exit(1);
	return (1);
}
