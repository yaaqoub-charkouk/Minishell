#include "built_ins.h"

int	built_in_unset(char *var, t_env **env)
{
	int		len;
	t_env	*prev;
	t_env	*curr;

	prev = NULL;
	curr = *env;
	len = ft_strlen(var);
	while (curr)
	{
		if (ft_strncmp(curr->content, var, len) == 0 
			&& curr->content[len] == '=')
		{
			if (prev == NULL)
				*env = curr->next;
			else
				prev->next = curr->next;
			free(curr->content);
			free(curr);
			return (0);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
