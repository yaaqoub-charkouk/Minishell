/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:48:32 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/20 21:48:33 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

void	copy_env(char **envp, t_list **env)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_lstadd_back(env, ft_lstnew(ft_strdup(envp[i])));
		i++;
	}
}

int	built_in_env(char **args, t_data *data)
{
	t_list	*current;

	if (args && args[1])
		return (errno = 2, perror("env"), 1);
	current = *data->envl;
	while (current)
	{
		if (ft_strchr(current->content, '='))
			printf("%s\n", current->content);
		current = current->next;
	}
	return (0);
}
