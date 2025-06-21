/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:48:03 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/21 11:25:47 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

void	expand_string(t_data *data, t_expand *expand)
{
	char	*pile;
	int		i;

	expand->arg = ft_strdup((*expand->args)[*expand->k]);
	pile = ft_strdup("");
	expand->pile = &pile;
	i = 0;
	while (expand->arg[i])
	{
		if (in_quotes(expand->arg[i],
				&expand->in_dquotes, &expand->in_squotes, &i))
			continue ;
		if (expand->arg[i] == '*' && ((expand->in_dquotes || expand->in_squotes)
				|| !ft_strcmp(*expand->args[0], "export")))
			expand->arg[i] = '\033';
		if (should_expand_variable(expand, i))
			expand_variable(data, expand, &i);
		else
			pile = accumulate_char(pile, expand->arg[i]);
		i++;
	}
	free(expand->arg);
	free((*expand->args)[*expand->k]);
	(*expand->args)[*expand->k] = pile;
}

void	expand_glob(t_expand *expand)
{
	int		k;
	char	**wilds;
	char	*pile;

	pile = ft_strdup("");
	expand->pile = &pile;
	k = 0;
	while ((*expand->args)[k])
	{
		if (ft_strchr((*expand->args)[k], '*'))
		{
			wilds = expand_wildcard((*expand->args)[k]);
			if (!wilds || wilds[0] == NULL)
			{
				k++;
				continue ;
			}
			expand->k = &k;
			insert_variable(expand, wilds, 0, 0);
		}
		free(pile);
		pile = ft_strdup("");
		k++;
	}
	free(pile);
}

void	expand_globs(t_expand *expand)
{
	int		k;
	char	*wild;

	if (!expand->is_ambiguous)
		return ;
	expand_glob(expand);
	k = 0;
	while ((*expand->args)[k])
	{
		wild = ft_strchr((*expand->args)[k], '\033');
		while (wild)
		{
			*wild = '*';
			wild = ft_strchr((*expand->args)[k], '\033');
		}
		k++;
	}
}

void	init_exp_data(t_expand *expand, char ***args, int *k, int *is_ambiguous)
{
	expand->args = args;
	expand->k = k;
	expand->in_dquotes = 0;
	expand->in_squotes = 0;
	expand->is_ambiguous = is_ambiguous;
	expand->pile = NULL;
	*k = 0;
}

char	**ft_expand(char *cmd, char **cmd_args, t_data *data, int *is_ambiguous)
{
	char		**args;
	int			k;
	t_expand	expand;

	if (!cmd_args && cmd)
		args = ft_split_pipex(cmd, ' ');
	else
		args = cmd_args;
	init_exp_data(&expand, &args, &k, is_ambiguous);
	if (!args)
		return (NULL);
	while (args[k])
	{
		expand_string(data, &expand);
		if (!args[k])
			break ;
		k++;
	}
	if (cmd_args)
		expand_globs(&expand);
	return (ignore_empty_args(args));
}
