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

char	*expand_string(char **args, int	k)
{
	int		in_dquotes;
	int		in_squotes;
	int		i;
	char	*pile;

	in_dquotes = 0;
	in_squotes = 0;
	i = 0;
	pile = ft_strdup(""); // init the pile to pile on it;
	while (args[k][i])
	{
		if (in_quotes(args[k][i], &in_dquotes, &in_squotes, &i))
			continue ; // if in quotes skip it ;
		
		printf("pile is  : %d %c %s\n", i, args[k][i],pile);
		pile = accumulate_char(pile, args[k][i]); // accumulate the char to pile;
		printf("pile is  : %d %c %s\n", i, args[k][i],pile);
		i++;
	}
	return (pile);
}

char	**ft_expand(char *cmd, t_data *data, int  *should_expand)
{
	char	**args;
	int		k;

	k = 0;
	args = ft_split_pipex(cmd, ' ');
	while (args[k])
	{
		args[k] = expand_string(args, k);
		printf("args %d : %s\n", k, args[k]);
		k++;
	}
	return (args);
}