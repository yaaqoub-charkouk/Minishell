/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1_a.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:39:08 by akharkho          #+#    #+#             */
/*   Updated: 2025/04/22 18:55:36 by akharkho         ###   ########.fr       */
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

int	has_unclosed_quotes(char *line, char quote)
{
	int	in_quote;
	int	i;

	in_quote = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == quote)
			in_quote = !in_quote;
		i++;
	}
	return (in_quote);
}

int	invalid_token(char *line)
{
	int	i;
	int	count_and;
	int	count_or;

	i = 0;
	while (line[i])
	{
		count_and = 0;
		count_or = 0;
		//cheecking the && operator
		if (line[i] == '&')
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
		else if (line[i] == '|')
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
int	handle_redirections(t_list *list)
{
	while (list)
	{
		if ((list->type == REDIRECTION_IN || list->type == REDIRECTION_OUT
				|| list->type == APPEND || list->type == HEREDOC) 
			&& list->next->type != CMD)
		{
			print_error("newline");
			return (1);
		}
		list = list->next;
	}
	return (0);
}

int	is_syntax_error(char *line, t_list *list)
{
	int		i;
	char	*temp;
	int		len;
	char	token[3];

	if (!line)
		return (1);
	if (handle_redirections(list))
		return (1);
	temp = ft_strdup(line);
	// checking differnt combinations of operators ||&& or &&|| etc ..
	if (invalid_token(temp))
		return(free(temp), 1);

	i = 0;
	while (is_space(temp[i]))
		i++;
	//check if the operator is first
	if (temp[i] == '|' || (temp[i] == '&' && temp[i + 1] == '&')
		|| (temp [i] == '|' && temp[i + 1] == '|'))
	{
		if ((temp[i] == '&' && temp[i + 1] == '&')
			|| (temp [i] == '|' && temp[i + 1] == '|'))
		{
			token[0] = temp[i];
			token[1] = temp[i + 1];
		}
		else
		{
			token[0] = temp[i];
			token[1] = '\0';
		}
		return (token[2] = '\0', print_error(token), free(temp), 1);
	}

	//check if the last thing is an operator
	len = ft_strlen(temp) - 1;
	while (len >= 0 && (temp[len] == ' ' || temp[len] == '\t'))
		len--;
	if (temp[len] == '|' || (temp[len] == '&' && temp[len - 1] == '&')
		|| (temp [len] == '|' && temp[len - 1] == '|'))
	{
		if ((temp[len] == '&' && temp[len - 1] == '&')
			|| (temp [len] == '|' && temp[len - 1] == '|'))
		{
			token[0] = temp[len];
			token[1] = temp[len - 1];
		}
		else
		{
			token[0] = temp[len];
			token[1] = '\0';
		}
		return (token[2] = '\0', print_error(token), free(temp), 1);
	}
	//checking quotes
	if (has_unclosed_quotes(line, '\''))
	{
		write(2, "minishell: syntax error unclosed single quote\n", 46);
		return (free(temp), 1);
	}
	if (has_unclosed_quotes(line, '\"'))
	{
		write(2, "minishell: syntax error unclosed double quotes\n", 47);
		return (free(temp), 1);
	}
	return (free(temp), 0);
}
