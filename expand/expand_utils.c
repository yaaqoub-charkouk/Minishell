/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:43:49 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/21 11:54:10 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

char	*accumulate_char(char *pile, char c)
{
	char	*new_str;
	int		len;

	if (!pile)
		return (NULL);
	len = ft_strlen(pile);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, pile, len + 1);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(pile);
	return (new_str);
}

int	in_quotes(char c, int *in_dquotes, int *in_squotes, int *i)
{
	if (c == '\"' && !*in_squotes)
	{
		*in_dquotes = !*in_dquotes;
		(*i)++;
		return (1);
	}
	if (c == '\'' && !*in_dquotes)
	{
		*in_squotes = !*in_squotes;
		(*i)++;
		return (1);
	}
	return (0);
}

int	ft_argslen(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}

int	should_expand_variable(t_expand *expand, int i)
{
	return (expand->is_ambiguous && expand->arg[i] == '$' 
		&& (ft_isalnum(expand->arg[i + 1]) || expand->arg[i + 1] == '_' 
			|| expand->arg[i + 1] == '?' || expand->arg[i + 1] == '$' 
			|| expand->arg[i + 1] == '0' || ((expand->arg[i + 1] == '\"'
			|| expand->arg[i + 1] == '\'') && !expand->in_dquotes) || expand->arg[i + 1] == '(')
			&& !expand->in_squotes);
}
