/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ignore_empty_args_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 15:54:35 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/22 11:21:13 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_bonus.h"

int	count_empties(char **args)
{
	int	k ;
	int	count;

	count = 0;
	k = 0;
	while (args[k])
	{
		if (*args[k] == '\a')
			count++;
		k++;
	}
	return (count);
}

char	**new_arguments(char ***new_args, char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (*args[i] == '\a')
			free(args[i]);
		else
		{
			(*new_args)[j] = args[i];
			j++;
		}
		i++;
	}
	free(args);
	return (*new_args);
}

char	**ignore_empty_args(char **args)
{
	int		count;
	int		total_args;
	char	**new_args;

	if (!args)
		return (NULL);
	total_args = ft_argslen(args);
	count = count_empties(args);
	if (count == 0)
		return (args);
	if (count == total_args)
	{
		free_matrix(args);
		return (NULL);
	}
	new_args = ft_calloc(total_args - count + 1, sizeof(char *));
	if (!new_args)
	{
		free_matrix(args);
		return (NULL);
	}
	return (new_arguments(&new_args, args));
}
