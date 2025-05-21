#include "parsing.h"

char    **expand(char *cmd)
{
	char    **args;
	int		should_expand;
	int		i;
	if (!cmd)
		return (NULL);
	i = 0;
	should_expand = 0;
	if (cmd[i] = '\"')
		should_expand = 1;
	
}