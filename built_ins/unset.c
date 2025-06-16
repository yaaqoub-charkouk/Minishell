#include "built_ins.h"

void	remove_env_var(t_data *data, char *arg)
{
	t_list	*curr;
	t_list	*prev;
	int		len;

	prev = NULL;
	curr = *data->envl;
	while (curr)
	{
		len = ft_strlen(arg);
		if (ft_strncmp(curr->content, arg, len) == 0)
		{
			if (prev == NULL)
				*data->envl = curr->next;
			else
				prev->next = curr->next;
			free(curr->content);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	built_in_unset(char **args, t_data *data)
{
	int		i;
	int		return_value;

	i = 1;
	return_value = 0;
	while (args[i])
	{
		if (is_unvalid_name(args[i], 1))
		{
			ft_putstr_fd("unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			i++;
			//change the return value to 1;
			return_value = 1;
			continue ;
		}
		else
			remove_env_var(data, args[i]);
		i++;
	}
	return (return_value);
}
