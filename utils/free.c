#include "utils.h"

void	free_matrix(char **args)
{
	int	i;

    if (!args)
        return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}
