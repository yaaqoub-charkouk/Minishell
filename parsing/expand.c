#include "parsing.h"

char	*accumulate_char(char *str, char c)
{
	char	*new_str;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, len + 1);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

int	in_quotes(char c, int *in_dquotes, int *in_squotes, int *i)
{
	if (c == '\"' && !*in_squotes)
	{
		*in_dquotes = !*in_dquotes;
		(*i)++;
		return (1);
	}	
	if (c == '\'' && !*in_dquotes)
	{
		*in_squotes = !*in_squotes;
		(*i)++;
		return (1);
	}
	return (0);
}

char	*get_var_value(t_env *env, char *value, int *i)
{
	int	len;
	
	len = 0;
	

	while (value[len] && ft_isalnum(value[len]))
	{
		len++;
	}
	if (i && len)
		*i += len - 1;
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0 
			&& env->content[len] == '=')
			return (env->content + len + 1); // should we re alloc for the returned string ;
		env = env->next;
	}
	return (ft_strdup("")); // return empty string if not found;
}
int	ft_argslen(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}
void	insert_variable(char ***args, char **pile, char *value, int *k)
{
	int		j;
	int		v;
	int		size;
	char	**var;
	char	**new_args;

	var = ft_split_pipex(value, ' '); // free it at the end of this function ;
	if (!var)
		return ;
	size = ft_argslen(*args) + ft_argslen(var);
	new_args = malloc(size * sizeof(char *));
	if (!new_args)
		return ;
	j = 0;
	v = 0;
	while (j < *k) // copy previous args; (*args)[j] && 
	{
		new_args[j] = ft_strdup((*args)[j]);
		j++;
	}// j == *k;
	new_args[(*k)++] = ft_strjoin(*pile, var[v++]);
	while (var[v]) // add the variable args ;
		new_args[(*k)++] = ft_strdup(var[v++]);
	v = *k;
	// (*k)--;
	while ((*args)[j])
		new_args[v++] = ft_strdup((*args)[j++]);
	new_args[v] = NULL;
	*pile = ft_strdup(new_args[(*k) - 1]);
	// (*k)--;
	// free (var) , free(old args);
	*args = new_args;
}

char	*expand_string(t_data *data, char ***args, int	*k)
{
	int		in_dquotes;
	int		in_squotes;
	int		i;
	char	*pile;
	char	*var_value;
	char	*arg;

	in_dquotes = 0;
	in_squotes = 0;
	i = 0;
	pile = ft_strdup(""); // init the pile to pile on it;
	arg = ft_strdup((*args)[*k]);

	while (arg[i])
	{
		if (in_quotes(arg[i], &in_dquotes, &in_squotes, &i))
			continue ; // if in quotes s*kip it ;
		/*----------------------------------------------------------------------------------------------------*/
		if (arg[i] == '$' && ft_isalnum(arg[i + 1]) && !in_squotes) // we have variable to expand ;
		{
			var_value = get_var_value(*data->envl, arg + i + 1, &i); // get the var value , and skip the var name ;
			if (in_dquotes || !ft_strchr(var_value, ' '))
				pile = ft_strjoin(pile, var_value);
			else
			{
				insert_variable(args, &pile, var_value, k);
				// free(var_value); var_value = NULL;
			}
		}
		else
			pile = accumulate_char(pile, arg[i]); // accumulate the char to pile;
		i++;
	}
	(*args)[*k] = pile; // k is the last known arg ;
	
	return (pile); // if you want to expand string by string ;
}

char	**ft_expand(char *cmd, t_data *data, int  *should_expand)
{
	char	**args;
	int		k;

	k = 0;
	args = ft_split_pipex(cmd, ' ');
	if (!args)
		return (NULL);
	while (args[k])
	{
		expand_string(data, &args, &k); // the function shold be void ; updating the args and k ;
		if(!args[k])
			break ;
		k++;
	}
	return (args);
}