#include "built_ins.h"

int	built_in_unset(char **args, t_env **env)
{
	int		len;
	int		i;
	t_env	*prev;
	t_env	*curr;

	i = 1;
	while (args[i])
	{
		prev = NULL;
		curr = *env;
		while (curr)
		{
			len = ft_strlen(args[i]);
			if (ft_strncmp(curr->content, args[i], len) == 0)

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
		i++;
	}
	return (0);
}
