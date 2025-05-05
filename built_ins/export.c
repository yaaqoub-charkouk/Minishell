#include "built_ins.h"

int	built_in_export(char **args, t_env **env)
{
	t_env	*curr;
	int		i;

	i = 1;
	curr = *env;
	if (!args[1])
	{
		while (curr)
		{
			printf("declare -x %s\n", curr->content);
			curr = curr->next;
		}
	}
	while (args[i])
	{
		ft_add_back(env, ft_new(args[i]));
		i++;
	}
	return (0);
}
