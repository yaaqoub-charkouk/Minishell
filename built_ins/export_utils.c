/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:48:49 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/20 21:48:50 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "built_ins.h"

t_list	*get_env_value(t_list *env, char *value)
{
	int	len;

	len = 0;
	while (value[len] && value[len] != '=' && value[len] != '+')
		len++;
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0
			&& (env->content[len] == '=' || env->content[len] == '\0'))
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	is_unvalid_name(char *value, int is_unset)
{
	int	i;

	i = 0;
	if (!value || (!ft_isalpha(value[i]) && value[i] != '_'))
		return (1);
	while (!is_unset && value[i] && value[i] != '=')
	{
		if (!ft_isalnum(value[i]) && value[i] != '_'
			&& value[i] !=  '+')
			return (1);
		if ((value[i] == '+' && value[i + 1] != '='))
			return (1);
		i++;
	}
	while (is_unset && value[i])
	{
		if (!ft_isalnum(value[i]) && value[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

void	ft_put_variable(char *var)
{
	write (1, "declare -x ", 11);
	while (*var && *var != '=')
	{
		write (1, var, 1);
		var++;
	}
	if (*var == '=')
	{
		write (1, var, 1);
		var++;
		write (1, "\"", 1);
		write (1, var, ft_strlen(var));
		write (1, "\"", 1);
	}
	write (1, "\n", 1);
}
