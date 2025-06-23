/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:48:32 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/23 12:59:45 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins_bonus.h"

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
	{
		ft_putstr_fd("minishell : ‘", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd("’: ", 2);
		ft_putstr_fd(strerror(2), 2);
		write(2, "\n", 1);
		return (127);
	}
	current = *data->envl;
	while (current)
	{
		if (ft_strchr(current->content, '='))
			printf("%s\n", current->content);
		current = current->next;
	}
	return (0);
}
