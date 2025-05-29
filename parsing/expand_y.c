#include "parsing.h"

char	**add_variable(char **args, char **string, char *var, int *k, int init_size)
{
	int		i;
	int		j;
	int		var_count;
	int		o_index;
	char	**new_args;
	char	**variable;

	var_count = count_words(var, ' ');
	new_args = malloc(sizeof(char *) * (init_size + var_count));
	variable = ft_split_pipex(var, ' ');
	i = 0;
	while (i < *k)
	{
		new_args[i] = ft_strdup(args[i]);
		i++;
	}
	*string = ft_strjoin(*string, variable[0]);
	new_args[*k] = *string;
	// i--;
	j = 1;
	while (j < var_count)
	{
		new_args[i] = ft_strdup(variable[j]);
		i++;
		j++;
	}
	// j--;
	// *string = variable[j];
	o_index = *k + 1;
	while (args[o_index])
	{
		new_args[i] = ft_strdup(args[o_index]);
		i++;
		o_index++;
	}
	new_args[i] = NULL;
	*k = *k + var_count - 1;
	return (new_args);
}

char    **expand(char *cmd, t_data *data)
{
	char	**args;
	char	*string;
	char	*new_str;
	int		i;
	int		k;
	int		in_dquotes;
	int		in_squotes;

	if (!cmd)
		return (NULL);
	in_dquotes = 0;
	in_squotes = 0;
	string = malloc(1);
	string[0] = '\0';
	new_str = NULL;

	k = 0;
	args = ft_split_pipex(cmd, ' ');// modify it later;
	while (args[k])
	{
		// printf("args %d %s\n", k, args[k]);
		i = 0;
		while (args[k][i])
		{
			if (args[k][i] == '\"' && !in_squotes)
			{
				in_dquotes = !in_dquotes;
				i++;
				continue ;
			}	
			if (args[k][i] == '\'' && !in_dquotes)
			{
				in_squotes = !in_squotes;
				i++;
				continue ;
			} // just determine if we are in quotes
			if (args[k][i] == '$' && args[k][i + 1] == '?' && !in_squotes)
			{
				if (g_sig)
				{
					data->exit_status = g_sig;
					g_sig = 0;
				}
				new_str = ft_itoa(data->exit_status);
				if (!new_str)
					new_str = ft_strdup("");
				string = ft_strjoin(string, new_str);
				free(new_str);
				i += 2;
				continue ;
			}

			if (args[k][i] == '$' && ft_isalnum(args[k][i + 1]) && !in_squotes)
			{
				new_str = get_env_content(*data->envl, &args[k][i + 1]);
				if (!new_str)
					new_str = ft_strdup("");
				if (in_dquotes)
					string = ft_strjoin(string, new_str);
				// else if (!in_dquotes)
				// 	args = add_variable(args, &string, new_str, &k, count_words(cmd, ' '));
				i++;
				while (args[k][i] && ft_isalnum(args[k][i]))// skip the variable name
				{
					if (ft_isdigit(args[k][i]))
					{
						i++;
						break;
					}
					i++;
				}
				continue ;
			}
			new_str = malloc(2);
			new_str[0] = args[k][i];
			new_str[1] = '\0';
			string = ft_strjoin(string, new_str);
			free(new_str);
			new_str = NULL;
			i++;
		}
		free(args[k]);

		args[k] = string;
		string = NULL;
		string = malloc(1);
		string[0] = '\0';

		k++;
	}
	
	return (args);
}