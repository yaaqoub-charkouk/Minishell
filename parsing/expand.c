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

char	*accumulate_char(char *pile, char c)
{
	char	*new_str;
	int		len;

	if (!pile)
		return (NULL);
	len = ft_strlen(pile);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, pile, len + 1);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(pile);
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

char	*get_var_value(t_list *env, char *value, int *i, int *word_boundary)
{
	int	len;
	
	len = 0;
	
	if (value[0] == '0')
		return ((*i)++, ft_strdup("minishell"));
	if (ft_isdigit(value[0]))
		return ((*i)++, ft_strdup(""));
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
			return (ft_strdup(env->content + len + 1)); // should we re alloc for the returned string ;
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

void	insert_variable(t_expand *expand, char **var, int space_flag , int word_boundary)
{
	int		j;
	int		v;
	int		size;
	int		var_count;
	char	**new_args;
	char	**to_free;

	size = 0;
	var_count = ft_argslen(var);
	if (var_count > 1)
		*expand->is_ambiguous = 1;
	size = ft_argslen(*expand->args) + var_count;
	if (space_flag && word_boundary)
		size++;
	new_args = ft_calloc((size + 1) , sizeof(char *));
	if (!new_args)
	{
		free_matrix(var);
		return ;
	}
	
	j = 0;
	v = 0;
	
	while (j < *expand->k)
	{
		new_args[j] = ft_strdup((*expand->args)[j]);
		j++;
	}
	
	new_args[j] = ft_strjoin(*expand->pile, var[v], 1);
	j++;
	v++;
	
	while (var[v])
	{
		new_args[j] = ft_strdup(var[v]);
		j++;
		v++;
	}
	free_matrix(var);
	v = *expand->k + 1;
	while ((*expand->args)[v])
	{
		new_args[j] = ft_strdup((*expand->args)[v]);
		j++;
		v++;
	}
	// free(*expand->pile);
	*expand->pile = ft_strdup(new_args[*expand->k + var_count - 1]);

	*expand->k = *expand->k + var_count - 1;

	if (space_flag && word_boundary)
	{
		free(*expand->pile);
		*expand->pile = ft_strdup("");
		(*expand->k)++;
	}
	// free old args and var
	to_free = *expand->args;
	*expand->args = new_args;
	free_matrix(to_free);
	to_free = NULL;
}
void	expand_variable(t_data *data, t_expand *expand, int *i)
{
	char	*var_value;
	int		word_boundary = 0;
	
	var_value = NULL;
	if (expand->arg[*i + 1] == '$')
		var_value = ft_strdup("1337");
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
		*expand->pile = ft_strjoin(*expand->pile, var_value, 1);
	else
		insert_variable(expand, ft_split_pipex(var_value, ' '), end_with_space(var_value), word_boundary);
	free(var_value);
}
void	expand_string(t_data *data, t_expand *expand)
{
	char		*pile;
	int			i;


	expand->arg = (*expand->args)[*expand->k];

	pile = ft_strdup(""); // init the pile to pile on it;
	expand->pile = &pile;
	i = 0;

	while (expand->arg[i])
	{
		if (in_quotes(expand->arg[i], &expand->in_dquotes, &expand->in_squotes, &i))
			continue ; // if in quotes s*kip it ;
		if (expand->arg[i] == '*' && ((expand->in_dquotes || expand->in_squotes) || !ft_strcmp(*expand->args[0], "export")))// if a wild card is found change it to an unprintable char
			expand->arg[i] = '\033';
		if (expand->is_ambiguous && expand->arg[i] == '$' && (ft_isalnum(expand->arg[i + 1]) || expand->arg[i + 1] == '_' || expand->arg[i + 1] == '?' || expand->arg[i + 1] == '$' || expand->arg[i + 1] == '0') && !expand->in_squotes) // we have variable to expand ;
			expand_variable(data, expand, &i);
		else
			pile = accumulate_char(pile, expand->arg[i]); // accumulate the char to pile;
		i++;
	}
	// free((*expand->args)[*expand->k]);
	// (*expand->args)[*expand->k]= NULL;
	(*expand->args)[*expand->k] = pile; // k is the last known arg ;
	
	// return ((*expand->args)[0]); // if you want to expand string by string ;
}
 void	expand_glob(t_expand *expand)
 {
	int	k = 0;
	char	**wilds;
	char	*pile;

	pile = ft_strdup("");
	// free(expand->pile);
	// expand->pile = NULL;
	expand->pile = &pile;
	while ((*expand->args)[k])
	{
		
		if (ft_strchr((*expand->args)[k], '*') || ft_strchr((*expand->args)[k], '\a'))
		{
			wilds = expand_wildcard((*expand->args)[k]);
			// print_args(wilds);
			if (!wilds || wilds[0] == NULL)
			{
				k++;
				continue ;
			}
			expand->k = &k;
			
			insert_variable(expand, wilds, 0, 0);
		}

		pile = ft_strdup("");
		k++;
	}
}

void	free_matrix(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

char	**ft_expand(char *cmd, char **cmd_args, t_data *data, int *is_ambiguous)
{
	char	**args;
	int		k;
	int		i;
	t_expand	expand;


	if (!cmd_args && cmd)
		args = ft_split_pipex(cmd, ' ');
	else
		args = cmd_args;

	expand.args = &args;
	expand.k = &k;
	expand.in_dquotes = 0;
	expand.in_squotes = 0;
	expand.is_ambiguous = is_ambiguous;
	expand.pile = NULL;
	k = 0;
	if (!args)
		return (NULL);
	while (args[k])
	{
		expand_string(data, &expand); // the function should be void ; updating the args and k ;
		if(!args[k])
			break ;
		k++;
	}
	if (cmd_args)
		expand_glob(&expand);
	k = 0;
	i = 0;
	while (args[k])
	{
		// while (args[k][i])
		// {
		// 	if (args[k][i] == '\a')
		// 		args[k][i] = '*';
		// 	i++;
		// }
		char	*wild = ft_strchr(args[k], '\033');
		if ( wild)
			*wild = '*';

		k++;
	}
	
	return (args);
}
