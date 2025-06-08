#include "built_ins.h"

t_env	*get_env_list(t_env *env, char *value)
{
	int	len;

	len = 0;
	while (value[len] && value[len] != '=' && value[len] != '+')
		len++;
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0 && (env->content[len] == '=' || env->content[len] == '\0'))
			return (env);
		env = env->next;
	}
	return (NULL);
}

int	is_unvalid_name(char *value)
{
	int	i;

	i = 0;
	if (!value || (!ft_isalpha(value[i]) && value[i] != '_'))
		return (1);
	while (value[i] && value[i] != '=' && value[i] != '+')
	{
		if (!ft_isalnum(value[i]) && value[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

void	ft_put_variable(char *var)
{
	write (1, "declare -x ", 11);
	while (*var && *var != '=')
	{
		write (1, var, 1);
		var++;
	}
	if (*var == '=')
	{
		write (1, var, 1);
		var++;
		write (1, "\"", 1);
		write (1, var, ft_strlen(var));
		write (1, "\"", 1);
	}
	write (1, "\n", 1);
}
