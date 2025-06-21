/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:51:19 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/21 20:31:44 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	handle_quotes(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !(*in_dquote))
		*in_squote = !(*in_squote);
	else if (c == '\"' && !(*in_squote))
		*in_dquote = !(*in_dquote);
	return ;
}

int	is_space(char c)
{
	return ((c == ' ' || c == '\t'));
}

void	print_error(char *token)
{
	write(2, "minishell: syntax error near unexpected token `", 47);
	if (token)
		write(2, token, ft_strlen(token));
	write(2, "'\n", 2);
}

int	operator_error(int count, int flag)
{
	if (count != 0 && flag)
	{
		if (count == 1)
			print_error("&");
		else
			print_error("&&");
		return (1);
	}
	else if (count != 0 && !flag)
	{
		if (count == 1)
			print_error("|");
		else
			print_error("||");
		return (1);
	}
	return (0);
}

int	is_operator(t_type_node type)
{
	return (type == PIPE || type == OR || type == AND);
}
