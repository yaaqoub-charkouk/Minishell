#include "built_ins.h"

int	built_in_echo(char **args, char **envp)
{
	int	nl_flag;
	int	i;
	(void)envp;
	i = 1;
	nl_flag = 0;
	if (ft_strncmp(args[i], "-n", 3) == 0)
	{
		nl_flag = 1;
		i++;
	}
	if (!args[i])
	{
		printf("\n");
		return (0);
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
	}
	if (nl_flag == 0)
		printf("\n");
	return (0);
}