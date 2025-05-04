#include "built_ins.h"

int	built_in_export(char *line, t_env **env)
{
	ft_add_back(env, ft_new(line));
	return (0);
}
