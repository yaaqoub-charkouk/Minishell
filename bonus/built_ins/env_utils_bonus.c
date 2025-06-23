/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:48:24 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/23 12:59:48 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins_bonus.h"

int	ft_listsize(t_list *lst)
{
	int	c;

	c = 0;
	while (lst)
	{
		lst = lst->next;
		c++;
	}
	return (c);
}

char	**env_struct_to_char(t_list *env)
{
	int		size;
	int		i;
	char	**env_char;

	i = 0;
	size = ft_listsize(env);
	env_char = malloc((size + 1) * sizeof(char *));
	if (!env_char)
		return (NULL);
	while (i < size)
	{
		env_char[i] = ft_strdup(env->content);
		env = env->next;
		i++;
	}
	env_char[i] = NULL;
	return (env_char);
}
