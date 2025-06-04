#include "parsing.h"

int	is_redirection(t_type_node type)
{
	return (type == REDIRECTION_IN || type == REDIRECTION_OUT 
			|| type == APPEND || type == HEREDOC);
}

int	is_operator(t_type_node type)
{
	return (type == PIPE || type == OR || type == AND);
}
