#include "parsing.h"

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

void	handle_quotes(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !(*in_dquote))
		*in_squote = !(*in_squote);
	else if (c == '\"' && !(*in_squote))
		*in_dquote = !(*in_dquote);
	return ;
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
		// cheecking the && operator
		if (line[i] == '&' && !in_squote && !in_dquote)
		{
			if (handle_and_operator(line, &i))
				return (1);
		}
		// cheecking the | and || operators
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
	// checking if it s at the EOL
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
	if (!line || !*line || !list)
		return (1337);
	if (handle_redirections(list))
		return (1);
	// checking differnt combinations of operators ||&& or &&|| etc ..
	if (invalid_token(line))
		return (1);
	if (check_op_start_end(list))
		return (1);
	// checking quotes && brackets
	if (handle_brackets(line) 
		|| has_unclosed_quotes(line) || has_special_char(line))
		return (1);
	return (0);
}
