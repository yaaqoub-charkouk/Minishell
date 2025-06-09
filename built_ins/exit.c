#include "built_ins.h"

int	built_in_exit(char **args, t_data *data)
{
	int	exit_status;

	if (args[1])
	{
		if (args[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			return (1);
		}
		exit_status = ft_atoi(args[1]);
		if (exit_status < 0 || exit_status > 255)
			exit_status = 255;
	}
	else
		exit_status = data->exit_status;
	exit(exit_status);
	return (0);
}
