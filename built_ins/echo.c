#include "built_ins.h"

int	built_in_echo(char **args)
{
	int	nl_flag;
	int	i;

	i = 1;
	nl_flag = 0;
	if (!args[i])
	{
		printf("\n");
		return (0);
	}
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		nl_flag = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (nl_flag == 0)
		printf("\n");
	return (0);
}
