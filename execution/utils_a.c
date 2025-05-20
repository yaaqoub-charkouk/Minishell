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
	return (type == REDIRECTION_IN || type == REDIRECTION_OUT 
			|| type == APPEND || type == HEREDOC);
}

int	is_operator(t_type_node type)
{
	return (type == PIPE || type == OR || type == AND);
}
