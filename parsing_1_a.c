/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1_a.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:39:08 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/26 17:23:10 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (line[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (line[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
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

int	invalid_token(char *line)
{
	int	i;
	int	in_squote;
	int	in_dquote;
	int	count_and;
	int	count_or;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (line[i])
	{
		count_and = 0;
		count_or = 0;
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (line[i] == '\'')
				in_squote = !in_squote;
			else
				in_dquote = !in_dquote;
		}
		//cheecking the && operator
		if (line[i] == '&' && !in_squote && !in_dquote)
		{
			while (line[i] == '&' || is_space(line[i]))
			{
				if (line[i] == '&')
					count_and++;
				i++;
			}
			if (count_and == 1 || count_and == 3)//this may be removed in the future single &
				return (print_error("&"), 1);
			else if (count_and != 2)
				return (print_error("&&"), 1);
			//check if the operators are mixed &&| &&|| etc..
			while (is_space(line[i]))
				i++;
			while (line[i] == '|')
			{
				count_or++;
				i++;
			}
			if (count_or != 0)
			{
				if (count_or == 1)
					print_error("|");
				else
					print_error("||");
				return (1);
			}
			count_or = 0;
			count_and = 0;
		}
		//cheecking the | and || operators
		else if (line[i] == '|' && !in_squote && !in_dquote)
		{
			while (line[i] == '|' || is_space(line[i]))
			{
				if (line[i] == '|')
					count_or++;
				i++;
			}
			if (count_or == 3)
				return (print_error("|"), 1);
			if (count_or > 3)
				return (print_error("||"), 1);
			//check if the operators are mixed ||& &&|| |& etc..
			while (is_space(line[i]))
				i++;
			while (line[i] == '&')
			{
				count_and++;
				i++;
			}
			if (count_and != 0)
			{
				if (count_and == 1)
					print_error("&");
				else
					print_error("&&");
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}
int	check_type_of_error(t_list *list)
{
	if (list->type == REDIRECTION_IN)
	{
		print_error("<");
		return (1);
	}
	if (list->type == HEREDOC)
	{
		print_error("<<");
		return (1);
	}
	if (list->type == REDIRECTION_OUT)
	{
		print_error(">");
		return (1);
	}
	if (list->type == APPEND)
	{
		print_error(">>");
		return (1);
	}
	return (0);
}
int	handle_redirections(t_list *list)
{
	while (list && list->next)
	{
		if ((list->type == REDIRECTION_IN || list->type == REDIRECTION_OUT
				|| list->type == APPEND || list->type == HEREDOC) 
			&& list->next->type != CMD)
			return (check_type_of_error(list), 1);
		list = list->next;
	}
	//checking if it s at the EOL
	if (check_type_of_error(list))
		return (1);
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
		if (line[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (line[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else if (line[i] == '(' && !in_squote && !in_dquote)
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
		list = list->next;
	if (list->type == AND || list->type == OR || list->type == PIPE)
	{
		print_error(list->content);
		return (1);
	}
	return (0);
}

int	is_syntax_error(char *line, t_list *list)
{
	// int		i;
	// int		len;
	// char	token[3];

	if (!line)
		return (1);
	if (handle_redirections(list))
		return (1);
	// checking differnt combinations of operators ||&& or &&|| etc ..
	if (invalid_token(line))
		return(1);
	if (check_op_start_end(list))
		return (1);
	// i = 0;
	// while (is_space(line[i]))
	// 	i++;
	// //check if the operator is first
	// if (line[i] == '|' || (line[i] == '&' && line[i + 1] == '&')
	// 	|| (line [i] == '|' && line[i + 1] == '|'))
	// {
	// 	if ((line[i] == '&' && line[i + 1] == '&')
	// 		|| (line [i] == '|' && line[i + 1] == '|'))
	// 	{
	// 		token[0] = line[i];
	// 		token[1] = line[i + 1];
	// 	}
	// 	else
	// 	{
	// 		token[0] = line[i];
	// 		token[1] = '\0';
	// 	}
	// 	return (token[2] = '\0', print_error(token), 1);
	// }

	//check if the last thing is an operator
	// len = ft_strlen(line) - 1;
	// while (len >= 0 && (line[len] == ' ' || line[len] == '\t'))
	// 	len--;
	// if (line[len] == '|' || (line[len] == '&' && line[len - 1] == '&')
	// 	|| (line [len] == '|' && line[len - 1] == '|'))
	// {
	// 	if ((line[len] == '&' && line[len - 1] == '&')
	// 		|| (line [len] == '|' && line[len - 1] == '|'))
	// 	{
	// 		token[0] = line[len];
	// 		token[1] = line[len - 1];
	// 	}
	// 	else
	// 	{
	// 		token[0] = line[len];
	// 		token[1] = '\0';
	// 	}
	// 	return (token[2] = '\0', print_error(token), 1);
	// }
	//checking quotes && brackets
	if (handle_brackets(line)
		|| has_unclosed_quotes(line) || has_special_char(line))
		return (1);
	return (0);
}
