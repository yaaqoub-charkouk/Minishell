#include "built_ins.h"

int	built_in_unset(char **args, t_data *data)
{
	int		len;
	int		i;
	int		return_value;
	t_env	*prev;
	t_env	*curr;

	i = 1;
	return_value = 0;
	while (args[i])
	{
		if (is_unvalid_name(args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			i++;
			//change the return value to 1;
			return_value = 1;
			continue ;
		}
		prev = NULL;
		curr = *data->envl;
		while (curr)
		{
			len = ft_strlen(args[i]);
			if (ft_strncmp(curr->content, args[i], len) == 0)

			{
				if (prev == NULL)
					*data->envl = curr->next;
				else
					prev->next = curr->next;
				free(curr->content);
				free(curr);
				return (return_value);
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
	return (return_value);
}
