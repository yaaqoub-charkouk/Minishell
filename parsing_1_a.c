#include "minishell.h"

int	is_syntax_error(char *line)
{
	int		i;
	char	*temp;
	char	*found;

	temp = ft_strdup(line);
	i = 0;
	while (temp[i] && (temp[i] == ' ' || temp[i] == '\t'))
		i++;
	if (temp[i] == PIPE || temp[i] == AND || temp [i] == OR)
		return (free(temp), 1);
	while (temp[i])
		i++;
	i--;
	while (temp[i] && (temp[i] == ' ' || temp[i] == '\t'))
		i--;
	if (temp[i] == PIPE || temp[i] == AND || temp [i] == OR)
		return (free(temp), 1);
	found = ft_strchr(line, '\'');
	if (*found == '\'')
	{
		found++;
		found = ft_strchr(line, '\'');
		if (*found != '\'')
			return (free(temp), 1);
	}
	return (0);
}
