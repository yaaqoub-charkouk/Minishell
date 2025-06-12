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

char	*get_var_value(t_env *env, char *value, int *i, int *word_boundary)
{
	int	len;
	
	len = 0;
	
	if (ft_isdigit(value[0]))
		return ((*i)++, ft_strdup(""));
	if (value[0] == '0')
		return ((*i)++, ft_strdup("minishell"));
	while (value[len] && (ft_isalnum(value[len]) || value[len] == '_'))
	{
		len++;
	}
	if (value[len])
		*word_boundary = 1;
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

int	end_with_space(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == ' ' && value[i + 1] == '\0')
			return (1);
		i++;
	}
	return (0);
}

void	insert_variable(char ***args, char **pile, char **var, int space_flag , int *k, int word_boundary, int *is_ambiguous)
{
	int		j;
	int		v;
	int		size;
	int		var_count;
	char	**new_args;

	size = 0;
	var_count = ft_argslen(var);
	if (var_count > 1)
		*is_ambiguous = 1;
	size = ft_argslen(*args) + var_count;
	if (space_flag && word_boundary)
		size++;
	new_args = calloc((size + 1) , sizeof(char *));
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
	
	v = *k + 1;
	while ((*args)[v])
	{
		new_args[j] = ft_strdup((*args)[v]);
		j++;
		v++;
	}
	// new_args[j] = NULL;
	
	*pile = ft_strdup(new_args[*k + var_count - 1]);
	*k = *k + var_count - 1;

	if (space_flag && word_boundary)
	{
		*pile = ft_strdup("");
		(*k)++;
	}
	
	// free old args and var
	*args = new_args;
}
void	expand_variable(t_data *data, t_expand *expand, int *i, int *is_ambiguous)
{
	char	*var_value;
	int		word_boundary = 0;
	
	if (expand->arg[*i + 1] == '$')
		var_value = ft_itoa(getpid());
	else if (expand->arg[*i + 1] == '?')
	{
		if (g_sig)
		{
			data->exit_status = g_sig;
			g_sig = 0;
		}
		var_value = ft_itoa(data->exit_status);
	}
	if (expand->arg[*i + 1] == '?' || expand->arg[*i + 1] == '$')
		(*i)++;
	else
		var_value = get_var_value(*data->envl, expand->arg + *i + 1, i, &word_boundary); // get the var value , and skip the var name ;
	if (expand->in_dquotes || !ft_strchr(var_value, ' '))
		*expand->pile = ft_strjoin(*expand->pile, var_value);
	else
		insert_variable(expand->args, expand->pile, ft_split_pipex(var_value, ' '), end_with_space(var_value), expand->k, word_boundary, is_ambiguous);
}
char	*expand_string(t_data *data, char ***args, int	*k, int *heredoc)
{
	t_expand	expand;
	char		*pile;
	int			i;

	
	expand.in_dquotes = 0;
	expand.in_squotes = 0;
	expand.k = k;
	expand.arg = (*args)[*k];
	expand.args = args;
	pile = ft_strdup(""); // init the pile to pile on it;
	expand.pile = &pile;
	i = 0;

	while (expand.arg[i])
	{
		if (in_quotes(expand.arg[i], &expand.in_dquotes, &expand.in_squotes, &i))
			continue ; // if in quotes s*kip it ;
		if (heredoc && expand.arg[i] == '$' && (ft_isalnum(expand.arg[i + 1]) || expand.arg[i + 1] == '_' || expand.arg[i + 1] == '?' || expand.arg[i + 1] == '$' || expand.arg[i + 1] == '0') && !expand.in_squotes) // we have variable to expand ;
			expand_variable(data, &expand, &i, heredoc);
		else
			pile = accumulate_char(pile, expand.arg[i]); // accumulate the char to pile;
		i++;
	}
	free(expand.arg);
	expand.arg = NULL;
	(*args)[*k] = pile; // k is the last known arg ;
	
	return ((*args)[0]); // if you want to expand string by string ;
}
 void	expand_glob(char ***args, int *is_ambiguous)
 {
	int	k = 0;
	char	**wilds;
	char	*pile;

	pile = ft_strdup("");
	while ((*args)[k])
	{
		if (ft_strchr((*args)[k], '*'))
		{
			wilds = expand_wildcard((*args)[k]);
			// print_args(wilds);
			if (wilds[0] == NULL)
			{
				k++;
				continue ;
			}
			insert_variable(args, &pile, wilds, 0, &k, 0, is_ambiguous);
			
		}
		pile = ft_strdup("");
		k++;
	}
 }
char	**ft_expand(char *cmd, char **cmd_args, t_data *data, int *is_ambiguous)
{
	char	**args;
	int		k;

	if (!cmd_args)
		args = ft_split_pipex(cmd, ' ');
	else
		args = cmd_args;
	k = 0;
	if (!args)
		return (NULL);
	while (args[k])
	{
		expand_string(data, &args, &k, is_ambiguous); // the function shold be void ; updating the args and k ;
		if(!args[k])
			break ;
		k++;
	}
	if (cmd_args)
		expand_glob(&args, is_ambiguous);
	return (args);
}
