#include "built_ins.h"

int	check_is_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	built_in_exit(char **args, t_data *data)
{
	long long	exit_status;
	int		overflow;

	if (args && args[1])
	{
		if (args[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			return (1);
		}
		exit_status = ft_atoi(args[1], &overflow);
		if (!check_is_num(args[1]) || overflow)
		{
			ft_putstr_fd("numeric argument required\n", 2);
			// exit(255);
			exit_status = 255;
		}
		printf("exit status %lld\n", exit_status);
		
	}
	else
		exit_status = data->exit_status;
	exit((unsigned char)exit_status % 256);
	return (0);
}
