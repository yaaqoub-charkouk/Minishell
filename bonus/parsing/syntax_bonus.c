/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 18:27:36 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/22 11:25:23 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_bonus.h"

int	has_special_char(char *line)
{
	int	i;
	int	in_squote;
	int	in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (line[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else if ((line[i] == '\\' || line[i] == ';')
			&& !in_squote && !in_dquote)
		{
			if (line[i] == '\\')
				print_error("\\");
			else
				print_error(";");
			return (1);
		}
		i++;
	}
	return (0);
}

int	has_unclosed_quotes(char *line)
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
		i++;
	}
	if (in_squote || in_dquote)
	{
		if (in_squote)
			write(2, "minishell: syntax error unclosed single quote\n", 46);
		else
			write(2, "minishell: syntax error unclosed double quotes\n", 47);
		return (1);
	}
	return (0);
}

int	handle_brackets(char *line)
{
	int	i;
	int	open;
	int	in_squote;
	int	in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	open = 0;
	while (line[i])
	{
		handle_quotes(line[i], &in_squote, &in_dquote);
		if (line[i] == '(' && !in_squote && !in_dquote)
			open++;
		else if (line[i] == ')' && !in_squote && !in_dquote)
		{
			if (!open)
				return (print_error(")"), 1);
			open--;
		}
		i++;
	}
	if (open)
		return (print_error("("), 1);
	return (0);
}

int	check_op_start_end(t_list *list)
{
	if (list->type == AND || list->type == OR || list->type == PIPE)
	{
		print_error(list->content);
		return (1);
	}
	while (list && list->next)
	{
		if ((list->type == P_CLOSE 
				&& (!is_operator(list->next->type)
					&& !(list->next->type == P_CLOSE)))
			|| (list->type == P_OPEN && list->next->type == P_CLOSE))
		{
			print_error(list->next->content);
			return (1);
		}
		list = list->next;
	}
	if (is_operator(list->type))
	{
		print_error(list->content);
		return (1);
	}
	return (0);
}

int	is_syntax_error(char *line, t_list *list)
{
	if (!line || !*line || !list)
		return (1337);
	if (handle_redirections(list))
		return (1);
	if (invalid_token(line))
		return (1);
	if (check_op_start_end(list))
		return (1);
	if (handle_brackets(line) 
		|| has_unclosed_quotes(line) || has_special_char(line)
		|| brackets_syntax(list) || handle_pipe_syntax(list))
		return (1);
	return (0);
}
