/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_spliting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:36:25 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/19 21:59:19 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	end_with_space(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] == '\0')
			return (1);
		i++;
	}
	return (0);
}

int	pre_insert_variable(t_expand *expand,
						char ***new_args, char **var, int *var_count)
{
	int	size;

	size = 0;
	*var_count = ft_argslen(var);
	if (*var_count > 1)
		*expand->is_ambiguous = 1;
	size = ft_argslen(*expand->args) + *var_count;
	if (expand->space_flag && expand->word_boundary)
		size++;
	*new_args = ft_calloc(size + 1, sizeof(char *));
	if (!*new_args)
	{
		free_matrix(var);
		perror("minishell :");
		return (0);
	}
	return (1);
}

int	copy_old_args(t_expand *expand, char ***new_args, char **var, int *v)
{
	int	j;

	j = ((*v = 0), 0);
	while (j < *expand->k)
	{
		(*new_args)[j] = ft_strdup((*expand->args)[j]);
		if (!(*new_args)[j])
			return (perror("minishell:"), 0);
		j++;
	}
	(*new_args)[j] = ft_strjoin(*expand->pile, var[*v], 1);
	if (!(*new_args)[j])
		return (perror("minishell:"), 0);
	j++;
	(*v)++;
	while (var[*v])
	{
		(*new_args)[j] = ft_strdup(var[*v]);
		if (!(*new_args)[j])
			return (perror("minishell:"), 0);
		j++;
		(*v)++;
	}
	free_matrix(var);
	return (j);
}

int	add_after_var_args(t_expand *expand, char ***new_args, int j, int var_count)
{
	int	v;

	v = *expand->k + 1;
	while ((*expand->args)[v])
	{
		(*new_args)[j] = ft_strdup((*expand->args)[v]);
		if (!(*new_args)[j])
			return (perror("minishell :"), 0);
		j++;
		v++;
	}
	*expand->pile = ft_strdup((*new_args)[*expand->k + var_count - 1]);
	if (!*expand->pile)
		return (perror("minishell:"), 0);
	*expand->k = *expand->k + var_count - 1;
	if (expand->space_flag && expand->word_boundary)
	{
		free(*expand->pile);
		*expand->pile = ft_strdup("");
		if (!*expand->pile)
			return (perror("minishell:"), 0);
		(*expand->k)++;
	}
	return (1);
}

void	insert_variable(t_expand *expand,
						char **var, int space_flag, int word_boundary)
{
	int		j;
	int		v;
	int		var_count;
	char	**new_args;

	if (!expand->is_ambiguous)
		return ;
	expand->space_flag = space_flag;
	expand->word_boundary = word_boundary;
	if (!pre_insert_variable(expand, &new_args, var, &var_count))
		return ;
	j = copy_old_args(expand, &new_args, var, &v);
	if (!j)
		return ;
	if (!add_after_var_args(expand, &new_args, j, var_count))
		return ;
	free_matrix(*expand->args);
	*expand->args = new_args;
}
