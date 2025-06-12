#include "built_ins.h"

void	handle_plus(char *plus, char *temp, t_data *data, t_list *curr)
{
	char	*joined;
	char	*joined2;

	*plus = '\0';
	if (curr)
	{
		if (ft_strchr(curr->content, '='))
			joined2 = ft_strjoin(curr->content, plus + 2, 0);
		else
		{
			joined = ft_strjoin(temp, "=", 0);
			joined2 = ft_strjoin(joined, plus + 2, 1);
		}
		free(curr->content);
		curr->content = joined2;
	}
	else
	{
		joined = ft_strjoin(temp, "=", 0);
		joined2 = ft_strjoin(joined, plus + 2, 1);
		ft_lstadd_back(data->envl, ft_lstnew(joined2));
		free(joined2);
	}
}

void	export_process_arg(char *arg, t_data *data)
{
	t_list	*curr;
	char	*plus;
	char	*equal;
	char	*temp;

	temp = ft_strdup(arg);
	curr = get_env_value(*data->envl, arg);
	plus = ft_strnstr(temp, "+=", ft_strlen(temp));
	equal = ft_strchr(temp, '=');
	if (plus)
		handle_plus(plus, temp, data, curr);
	else if (equal)
	{
		if (curr)
		{
			// free old content
			free(curr->content);
			curr->content = ft_strdup(arg);
		}
		else
			ft_lstadd_back(data->envl, ft_lstnew(arg));
	}
	else if (!curr)
		ft_lstadd_back(data->envl, ft_lstnew(arg));
	// free(temp);
}

void	print_export(t_data *data)
{
	t_list	*curr;

	curr = *data->envl;
	while (curr)
	{
		ft_put_variable(curr->content);
		curr = curr->next;
	}
}

int	built_in_export(char **args, t_data *data)
{
	int		i;
	int		return_value;

	i = 1;
	return_value = 0;
	if (!args[1])
	{
		print_export(data);
		return (0);
	}
	while (args[i])
	{
		if (is_unvalid_name(args[i]))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			i++;
			return_value = 1;
			continue ;
		}
		export_process_arg(args[i], data);
		i++;
	}
	return (return_value);
}
