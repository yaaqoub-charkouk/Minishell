/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:48:21 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 17:38:59 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins_bonus.h"

int	check_option(char *line)
{
	int	i;

	i = 0;
	if (line[i] == '-')
	{
		i++;
		while (line[i])
		{
			if (line[i] != 'n')
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

int	built_in_echo(char **args)
{
	int	nl_flag;
	int	i;

	i = 1;
	nl_flag = 0;
	if (!args[i])
	{
		printf("\n");
		return (0);
	}
	while (args[i] && check_option(args[i]))
	{
		nl_flag = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (nl_flag == 0)
		printf("\n");
	return (0);
}
