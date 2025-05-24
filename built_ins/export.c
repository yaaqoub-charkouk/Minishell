#include "built_ins.h"

t_env	*get_env_list(t_env *env, char *value)
{
	int	len;

	len = 0;
	while (value[len] && value[len] != '=' && value[len] != '+')
		len++;
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0)
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

void	ft_putvariable(char *var)
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

int	built_in_export(char **args, t_env **env)
{
	t_env	*curr;
	int		i;
	int		return_value;

	i = 1;
	return_value = 0;
	curr = *env;
	if (!args[1])
	{
		while (curr)
		{
			ft_putvariable(curr->content);
			curr = curr->next;
		}
	}
	while (args[i])
	{
		char	*plus;
		char	*equal;
		char	*name;
		char *value;
		char *temp;
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
		temp = ft_strdup(args[i]);
		curr = get_env_list(*env, args[i]);
		plus = ft_strnstr(temp, "+=", ft_strlen(temp));
		equal = ft_strchr(temp, '=');
		if (plus)
		{
			*plus = '\0';
			name = temp;
			value = plus + 2;
			value = ft_strtrim(value, "\"", "\'");
			if (curr)
			{
				char *prev_value  = ft_strchr(curr->content, '=');
				if (prev_value)
				{
					char *joined = ft_strjoin(prev_value + 1, value);
					char *joined2 = ft_strjoin(temp, "=");
					char *last_joined = ft_strjoin(joined2, joined);
					// free old content
					free(curr->content);
					curr->content = last_joined;
					//free the temp variables
					// free(joined);
					// free(joined2);
					// free(value);
				}
				else
				{
					char *joined_str = ft_strjoin(temp, "=");
					char *joined_str_final = ft_strjoin(joined_str, value);
					// free old content
					free(curr->content);
					curr->content = joined_str_final;
					// free the temp variables
					// free(joined_str);
					// free(joined_str_final);
				}
			}
			else
			{
				char *joined_str = ft_strjoin(temp, "=");
				char *joined_str_final = ft_strjoin(joined_str, value);
				ft_add_back(env, ft_new(joined_str_final));
				// free the temp variables
				// free(joined_str);
				// free(joined_str_final);
			}
		}
		else if (equal)
		{
			if (curr)
			{
				// free old content
				free(curr->content);
				curr->content = ft_strdup(args[i]);
			}
			else
				ft_add_back(env, ft_new(args[i]));
		}
		else if (!curr)
			ft_add_back(env, ft_new(args[i]));
		i++;
		// free the temp variable after using it
		// free(temp);
	}
	return (return_value);
}
