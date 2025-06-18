#include "expand.h"

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
	if (value[len] && word_boundary)
		*word_boundary = 1;
	*i += len;
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0 
			&& env->content[len] == '=')
			return (ft_strdup(env->content + len + 1));
		env = env->next;
	}
	return (ft_strdup(""));
}
void	expand_special_variable(t_data *data, t_expand *expand, int *i, char **var_value)
{
	if (expand->arg[*i + 1] == '$')
		*var_value = ft_strdup("1337");
	else if (expand->arg[*i + 1] == '?')
	{
		if (g_sig)
		{
			data->exit_status = g_sig;
			g_sig = 0;
		}
		*var_value = ft_itoa(data->exit_status);
	}
}
void	expand_variable(t_data *data, t_expand *expand, int *i)
{
	char	*var_value;
	int		word_boundary = 0;
	
	var_value = NULL;
	expand_special_variable(data, expand, i, &var_value);
	if (expand->arg[*i + 1] == '?' || expand->arg[*i + 1] == '$')
		(*i)++;
	else
		var_value = get_var_value(*data->envl, expand->arg + *i + 1, i, &word_boundary); // get the var value , and skip the var name ;
	if (*var_value == '\0')
		*expand->is_ambiguous = 1;
	if (expand->in_dquotes || !ft_strchr(var_value, ' '))
		*expand->pile = ft_strjoin(*expand->pile, var_value, 1);
	else
		insert_variable(expand, ft_split_pipex(var_value, ' '), end_with_space(var_value), word_boundary);
	free(var_value);
}
