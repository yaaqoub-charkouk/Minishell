#include "minishell.h"

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

int	is_syntax_error(char *line)
{
	int		i;
	char	*temp;

	if (!line)
		return (1);
	temp = ft_strdup(line);
	i = 0;
	while (temp[i] && (temp[i] == ' ' || temp[i] == '\t'))
		i++;
	if (temp[i] == '|' || (temp[i] == '&' && temp[i + 1] == '&')
		|| (temp [i] == '|' && temp[i + 1] == '|'))
		return (free(temp), 1);
	temp += ft_strlen(temp) - 1;
	while (i >= 0 && (temp[i] == ' ' || temp[i] == '\t'))
		i--;
	if (temp[i] == '|' || (temp[i] == '&' && temp[i + 1] == '&')
		|| (temp [i] == '|' && temp[i + 1] == '|'))
		return (free(temp), 1);
	if (has_unclosed_quotes(line, '\'') || has_unclosed_quotes(line, '\"'))
		return (free(temp), 1);
	return (free(temp), 0);
}
