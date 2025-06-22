/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:44:49 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/22 13:34:11 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

char	*get_var_value(t_list *env, char *value, int *i, int *word_boundary)
{
	int	len;

	len = 0;
	if (value[0] == '0')
		return ((*i)++, ft_strdup("minishell"));
	if (ft_isdigit(value[0]))
		return ((*i)++, ft_strdup(""));
	while (value[len] && (ft_isalnum(value[len]) || value[len] == '_'))
		len++;
	if (value[len] && word_boundary)
		*word_boundary = 1;
	*i += len;
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0 
			&& env->content[len] == '=')
			return (ft_strdup(env->content + len + 1));
		env = env->next;
	}
	return (ft_strdup(""));
}

void	expand_special_var(t_data *data,
							t_expand *expand, int *i, char **var_value)
{
	if (expand->arg[*i + 1] == '$')
		*var_value = ft_strdup("1337");
	else if (expand->arg[*i + 1] == '?')
	{
		if (g_sig)
		{
			data->exit_status = g_sig;
			g_sig = 0;
		}
		*var_value = ft_itoa(data->exit_status);
	}
}

void	expand_variable(t_data *data, t_expand *expand, int *i)
{
	char	*var_value;
	int		word_boundary;

	var_value = NULL;
	word_boundary = 0;
	expand_special_var(data, expand, i, &var_value);
	if (expand->arg[*i + 1] == '(')
		var_value = expand_cmd_substitution(data, expand, i);
	else if (expand->arg[*i + 1] == '?' || expand->arg[*i + 1] == '$')
		(*i)++;
	else
		var_value = get_var_value(*data->envl, 
				expand->arg + *i + 1, i, &word_boundary);
	if (*var_value == '\0' && !**expand->pile && !expand->arg[*i + 1])
		*expand->is_ambiguous = 1;
	if (!*var_value && !**expand->pile && !expand->arg[*i + 1])
		*var_value = '\a';
	if (expand->in_dquotes || !ft_strchr(var_value, ' '))
		*expand->pile = ft_strjoin(*expand->pile, var_value, 1);
	else
		insert_variable(expand, ft_split_pipex(var_value, ' '),
			end_with_space(var_value), word_boundary);
	free(var_value);
}
