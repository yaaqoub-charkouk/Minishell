#include "parsing.h"



char    **expand(char *cmd, t_data *data)
{
	char	**args;
	char	*string;
	char	*new_str;
	char	*temp;
	int		i;
	int		j;
	int		k;
	int		in_dquotes;
	int		in_squotes;

	in_dquotes = 0;
	in_squotes = 0;
	string = malloc(1);
	string[0] = '\0';
	new_str = NULL;

	j = 0;
	k = 0;
	args = ft_split_pipex(cmd, ' ');// modify it later;
	while (args[k])
	{
		// printf("args %d %s\n", k, args[k]);
		i = 0;
		// free(string);
		
		while (args[k][i])
		{

			// printf("args %d %d %c\n", k, i, args[k][i]);
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
			if (args[k][i] == '$' && !in_squotes)
			{
				printf("expanding variable\n");
				new_str = get_env_content(*data->envl, &args[k][i + 1]);
				if (!new_str)
					new_str = ft_strdup("");

				string = ft_strjoin(string, new_str);
				i++;
				while (args[k][i] && args[k][i] != '"' && args[k][i] != ' ')
				{
					i++;
				}
				
				continue ;
			}
			if (in_squotes)
			{
				new_str = malloc(2);
				new_str[0] = args[k][i];
				new_str[1] = '\0';
			}
			else if (in_dquotes)
			{
				// printf("iN DOUBLE QUOTES\n");
				new_str = malloc(2);
				new_str[0] = args[k][i];
				new_str[1] = '\0';
			}
			else
			{
				new_str = malloc(2);
				new_str[0] = args[k][i];
				new_str[1] = '\0';
			}
			string = ft_strjoin(string, new_str);
			free(new_str);
			new_str = NULL;
			i++;
		}
		free(args[k]);
		// temp = ft_strdup(string);
		temp = NULL;
		args[k] = string;
		// free(string);
		string = NULL;
		string = malloc(1);
		string[0] = '\0';

		k++;
	}
	 i = 0;
	while (args[i])
	{
		printf("args: [%d] %s\n", i, args[i]);
		i++;
	}
	
	return (args);
}