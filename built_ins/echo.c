#include "built_ins.h"

void	check_special(char *arg, t_env *env)
{
	char	*value;
	char	*str;

	if (ft_strchr(arg, '$'))
	{
		if (*arg == '\"' || *arg == '$')
		{
			while (*arg == '\"')
				arg++;
			value = arg;
			while (*arg && *arg != '\"')
				arg++;
			*arg = '\0';
			if (!value)
				return ;
			if (*value == '$')
				value++;
			str = get_env_content(env, value);
			if (!str)
				return ;
			printf("%s", str);
		}
	}
	else
		printf("%s", arg);
}

int	built_in_echo(char **args, t_env *env)
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
		check_special(args[i], env);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (nl_flag == 0)
		printf("\n");
	return (0);
}
