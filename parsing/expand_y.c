#include "parsing.h"

char	**add_variable(char **args, char **string, char *var_value, int *k, int init_size)
{
	int		i;
	int		j;
	int		var_count;
	int		o_index;
	char	**new_args;
	char	**variable;

	var_count = count_words(var_value, ' ');
	new_args = malloc(sizeof(char *) * (init_size + var_count + 1));
	variable = ft_split_pipex(var_value, ' ');
	if (!variable)
		printf("variable is NULL");
	i = 0;
	while (i < *k)
	{
		new_args[i] = ft_strdup(args[i]);
		i++;
	}
	if (var_count == 0)
		new_args[*k] = *string;
	else
	{
		*string = ft_strjoin(*string, variable[0]);
		new_args[*k] = *string;
	}

	i++;
	j = 1;
	while (j < var_count)
	{
		new_args[i] = ft_strdup(variable[j]);
		i++;
		j++;
	}

	o_index = *k + 1;
	while (args[o_index])
	{
		new_args[i] = ft_strdup(args[o_index]);
		i++;
		o_index++;
	}
	new_args[i] = NULL;
	*k = *k + var_count - 1;
	j = 0;
	while (new_args[j])
	{
		printf("new args %d : %s \n", j, new_args[j]);
		j++;
	}
	return (new_args);
}

char    **expand(char *cmd, t_data *data)
{
	char	**args;
	char	*string;
	char	*new_str;
	char	*temp;
	int		i;
	int		k;
	int		j = 0;
	int		in_dquotes;
	int		in_squotes;
	int		is_expanded = 0;

	if (!cmd)
		return (NULL);
	in_dquotes = 0;
	in_squotes = 0;
	string = malloc(1);
	string[0] = '\0';
	new_str = NULL;

	k = 0;
	args = ft_split_pipex(cmd, ' ');// modify it later;
	printf("cmd is %s %s\n", args[0], args[1]);
	while (args[k])
	{
		is_expanded = 0;
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
				printf("------------- %c -------------\n", args[k][i]);
				new_str = get_env_content(*data->envl, &args[k][i + 1]);
				if (!new_str)
					new_str = ft_strdup("");
				if (in_dquotes)
					string = ft_strjoin(string, new_str);
				i++;
				while (args[k][i] && ft_isalnum(args[k][i])) // skip the variable name
				{
					if (ft_isdigit(args[k][i]))
					{
						i++;
						break;
					}
					i++;
				}
				if (new_str[0] == '\0')
					continue ;
				temp = &args[k][i];
				j = 0;
				if (!in_dquotes && new_str[0] != '\0')
				{
					args = add_variable(args, &string, new_str, &k, count_words(cmd, ' '));// k =0;
					i = 0;
					is_expanded = 1;
					if (temp[0] != '\0')
						args[k] = ft_strjoin(args[k], temp); // args[k] is the last var arg	
					string = ft_strdup("");
				}
				
			}
			
			new_str = malloc(2); // accumulate the current char to arg
			new_str[0] = args[k][i]; // heap buffer overflow 
			new_str[1] = '\0';

			string = ft_strjoin(string, new_str); // accumulate the current char to arg
			free(new_str);
			new_str = NULL;
			i++;
		}
		if (is_expanded)
			string = ft_strdup("");
		else
		{
			args[k] = string;
			string = NULL;
			string = malloc(1);
			string[0] = '\0';
			k++;
		}

	}
	j = 0;
	while(args[j])
	{
		printf("%d %s\n", j, args[j]);
		j++;
	}
	
	printf("cmd is %s %s\n", args[0], args[1]);
	return (args);
}
//export a="'ls'"
//echo $'PWD' ----> PWD (correct output)