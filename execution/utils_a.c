#include "execution.h"

void	free_split(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	free(str);
	str = NULL;
}

int	is_redirection(t_type_node type)
{
	if (type == REDIRECTION_IN || type == REDIRECTION_OUT 
			|| type == APPEND || type == HEREDOC)
		return (1);
	return (0);
}
