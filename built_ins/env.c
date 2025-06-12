#include "built_ins.h"

void	copy_env(char **envp, t_list **env)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_lstadd_back(env, ft_lstnew(ft_strdup(envp[i])));
		i++;
	}
}

int	built_in_env(t_data *data)
{
	t_list	*current;

	current = *data->envl;
	while (current)
	{
		if (ft_strchr(current->content, '='))
			printf("%s\n", current->content);
		current = current->next;
	}
	return (0);
}
