#include "parsing.h"

void	print_args(char **args)
{
	int i = 0;
	while (args[i])
	{
		printf("new_args[%d] : %s\n", i, args[i]);
		i++;
	}
}

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
	
	if (value[0] == '0')
		return ((*i)++, ft_strdup("minishell"));
	while (value[len] && (ft_isalnum(value[len]) || value[len] == '_'))
	{
		len++;
	}
	// if (i && len)
		*i += len;
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
	int		l;
	int		size;
	int		var_count;
	char	**var;
	char	**new_args;

	printf("insert variable k is %d\n", *k);
	var = ft_split_pipex(value, ' ');
	if (!var)
		return ;
	
	var_count = ft_argslen(var);
	size = ft_argslen(*args) + var_count;
	new_args = malloc((size + 1) * sizeof(char *));
	if (!new_args)
		return ;
	
	j = 0;
	v = 0;
	
	while (j < *k)
	{
		new_args[j] = ft_strdup((*args)[j]);
		j++;
	}
	
	new_args[j] = ft_strjoin(*pile, var[v]);
	j++;
	v++;
	
	while (var[v])
	{
		new_args[j] = ft_strdup(var[v]);
		j++;
		v++;
	}
	
	l = *k + 1;
	while ((*args)[l])
	{
		new_args[j] = ft_strdup((*args)[l]);
		j++;
		l++;
	}
	new_args[j] = NULL;
	
	*pile = ft_strdup(new_args[*k + var_count - 1]);
	
	*k = *k + var_count - 1;
	
	printf("update k to %d\n", *k);
	printf("pile at k=%d : %s\n", *k, *pile);
	
	// free old args and var
	*args = new_args;
}
void	expand_variable(t_data *data, char ***args, char *arg, char **pile, int *k, int *i,int in_dquotes, int in_squotes)
{
	char	*var_value;

	var_value = get_var_value(*data->envl, arg + *i + 1, i); // get the var value , and skip the var name ;
	if (arg[*i + 1] == '$')
		var_value = ft_strdup("1337");
	if (arg[*i + 1] == '?')
	{
		if (g_sig)
		{
			data->exit_status = g_sig;
			g_sig = 0;
		}
		var_value = ft_itoa(data->exit_status);
	}
	if (arg[*i + 1] == '?' || arg[*i + 1] == '$')
		(*i)++;
	if (in_dquotes || !ft_strchr(var_value, ' '))
		*pile = ft_strjoin(*pile, var_value);
	else
	{
		insert_variable(args, pile, var_value, k);
	}
}
char	*expand_string(t_data *data, char ***args, int	*k)
{
	int		in_dquotes;
	int		in_squotes;
	int		i;
	char	*pile;
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
		if (arg[i] == '$' && (ft_isalnum(arg[i + 1]) || arg[i + 1] == '_' || arg[i + 1] == '?' || arg[i + 1] == '$' || arg[i + 1] == '0') && !in_squotes) // we have variable to expand ;
		{
			expand_variable(data, args, arg, &pile, k, &i, in_dquotes, in_squotes);
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