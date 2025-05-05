#include "built_ins.h"

int	ft_envsize(t_env *lst)
{
	int	c;

	c = 0;
	while (lst)
	{
		lst = lst->next;
		c++;
	}
	return (c);
}

char    **env_struct_to_char(t_env  *env) // the function to convert t_env struct to char ** ;
{
	int		size;
	int		i;
	char	**env_char;

	i = 0;
	size =  ft_envsize(env);
	env_char = malloc(size * sizeof(t_env *));
	while (i < size)
	{
		env_char[i] = ft_strdup(env->content);
		env = env->next;
		i++;
	}
	return (env_char); // need to be freed ;
}
