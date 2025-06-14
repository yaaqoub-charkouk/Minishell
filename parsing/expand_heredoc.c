#include "parsing.h"

int	should_expand(char *line, int i)
{
	return ( line[i] == '$' 
		&& (ft_isalnum(line[i + 1]) || line[i + 1] == '_' 
		|| line[i + 1] == '?' || line[i + 1] == '$' 
		|| line[i + 1] == '0'));
}

static void	expand_variable(t_data *data, char *line, int *i, char **pile)
{
	char	*var_value;
	
	var_value = NULL;
	if (line[*i + 1] == '$')
		var_value = ft_strdup("1337");
	else if (line[*i + 1] == '?')
	{
		if (g_sig)
		{
			data->exit_status = g_sig;
			g_sig = 0;
		}
		var_value = ft_itoa(data->exit_status);
	}
	if (line[*i + 1] == '?' || line[*i + 1] == '$')
		(*i)++;
	else
		var_value = get_var_value(*data->envl, line + *i + 1, i, NULL); // get the var value , and skip the var name ;
	if (!ft_strchr(var_value, ' '))
		*pile = ft_strjoin(*pile, var_value, 1);
	free(var_value);
}

char *expand_heredoc(char *line, t_data *data)
{
	int i;
	char *pile;

	pile = ft_strdup("");
	i =0;
	while (line[i])
	{
		if (should_expand(line, i))
			expand_variable(data, line, &i, &pile);
		else
			pile = accumulate_char(pile, line[i]);
		i++;
	}
	return (pile);
}
