/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:27:39 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 17:40:19 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_bonus.h"

int	handle_and_operator(char *line, int *i)
{
	int	count_and;
	int	count_or;

	count_and = 0;
	count_or = 0;
	while (line[*i] == '&' || is_space(line[*i]))
	{
		if (line[*i] == '&')
			count_and++;
		(*i)++;
	}
	if (count_and == 1 || count_and == 3)
		return (print_error("&"), 1);
	else if (count_and != 2)
		return (print_error("&&"), 1);
	while (is_space(line[*i]))
		(*i)++;
	while (line[*i] == '|')
	{
		count_or++;
		(*i)++;
	}
	if (operator_error(count_or, 0))
		return (1);
	return (0);
}

int	handle_or_operator(char *line, int *i)
{
	int	count_and;
	int	count_or;

	count_and = 0;
	count_or = 0;
	while (line[*i] == '|' || is_space(line[*i]))
	{
		if (line[*i] == '|')
			count_or++;
		(*i)++;
	}
	if (count_or == 3)
		return (print_error("|"), 1);
	if (count_or > 3)
		return (print_error("||"), 1);
	while (is_space(line[*i]))
		(*i)++;
	while (line[*i] == '&')
	{
		count_and++;
		(*i)++;
	}
	if (operator_error(count_and, 1))
		return (1);
	return (0);
}

int	handle_pipe_syntax(t_list *list)
{
	t_list	*current;
	t_list	*prev;

	current = list;
	prev = NULL;
	while (current)
	{
		if (current->type == PIPE)
		{
			if (prev->type == PIPE || prev->type == AND || prev->type == OR)
			{
				print_error("|");
				return (1);
			}
			if (current->next->type == PIPE || current->next->type == AND 
				|| current->next->type == OR)
			{
				print_error(current->next->content);
				return (1);
			}
		}
		prev = current;
		current = current->next;
	}
	return (0);
}

int	invalid_token(char *line)
{
	int	i;
	int	in_squote;
	int	in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (line[i])
	{
		handle_quotes(line[i], &in_squote, &in_dquote);
		if (line[i] == '&' && !in_squote && !in_dquote)
		{
			if (handle_and_operator(line, &i))
				return (1);
		}
		else if (line[i] == '|' && !in_squote && !in_dquote)
		{
			if (handle_or_operator(line, &i))
				return (1);
		}
		else
			i++;
	}
	return (0);
}

int	brackets_syntax(t_list *list)
{
	while (list && list->next)
	{
		if (list->type == P_CLOSE && list->next->type == CMD)
			return (print_error(list->next->content), 1);
		if (list->type == CMD && list->next->type == P_OPEN)
			return (print_error("("), 1);
		list = list->next;
	}
	return (0);
}
