#include "minishell.h"


int	is_space(char c)
{
	return ((c == ' ' || c == '\t'));
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
			{
				printf("minishell: syntax error near unexpected token `&'\n");
				return (1);
			}
			else if (count_and != 2)
			{
				printf("minishell: syntax error near unexpected token `&&'\n");
				return (1);
			}
			//check if the operators are mixed &&| &&|| etc..
			while (is_space(line[i]))
				i++;
			while(line[i] == '|')
			{
				count_or++;
				i++;
			}
			if (count_or != 0)
			{
				if (count_or == 1)
					printf("minishell: syntax error near unexpected token `|'\n");
				else
					printf("minishell: syntax error near unexpected token `||'\n");
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
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return (1);
			}
			if (count_or > 3)
			{
				printf("minishell: syntax error near unexpected token `||'\n");
				return (1);
			}
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
					printf("minishell: syntax error near unexpected token `&'\n");
				else
					printf("minishell: syntax error near unexpected token `&&'\n");
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}


int	is_syntax_error(char *line)
{
	int		i;
	char	*temp;
	int		len;

	if (!line)
		return (1);
	temp = ft_strdup(line);
	// checking differnt combinations of operators ||&& or &&|| etc ..
	if (invalid_token(temp))
		return(free(temp), 1);

	i = 0;
	while (is_space(temp[i]))
		i++;
	//check if the operator is first
	if (temp[i] == '|' ||(temp[i] == '&' && temp[i + 1] == '&')
		|| (temp [i] == '|' && temp[i + 1] == '|'))
	{
		if ((temp[i] == '&' && temp[i + 1] == '&')
			|| (temp [i] == '|' && temp[i + 1] == '|'))
		{
			printf("minishell: syntax error near unexpected");
			printf("token `%c%c'\n", temp[i], temp[i + 1]);
		}
		else
		{
			printf("minishell: syntax error near unexpected ");
			printf("token `%c'\n", temp[i]);
		}
		return (free(temp), 1);
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
			printf("minishell: syntax error near unexpected ");
			printf("token `%c%c'\n", temp[len], temp[len - 1]);
		}
		else
		{
			printf("minishell: syntax error near unexpected ");
			printf("token `%c'\n", temp[len]);
		}
		return (free(temp), 1);
	}
	//checking quotes
	if (has_unclosed_quotes(line, '\''))
	{
		printf("minishell: syntax error unclosed single quote\n");
		return (free(temp), 1);
	}
	if (has_unclosed_quotes(line, '\"'))
	{
		printf("minishell: syntax error unclosed double quotes\n");
		return (free(temp), 1);
	}
	return (free(temp), 0);
}
