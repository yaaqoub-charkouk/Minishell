#include "minishel.h"

void	function(t_list **list, char *line)
{
	int	i;
	int j;
	
	j = 0;
	i = 0;
	while (line[i])
	{
		(*list)->cmd[j][i] = line[i];
		if (line[i] == '|')
			j++;
		i++;
	}
}

int main(int    ac, char **av, char **env)
{
	char    *line;
	t_list	*list;

	(void)ac;
	(void)av;
	(void)env;

	while(1337)
	{
		line = readline("minishell-1.0 $ ");
		function(&list, line);
		printf("the command is : %s\n", line);
	}
	return (0);
}

// we have to parse the line, 
// syntax error
// ghanheto dik commandat f wahd tree , 
// after that , we have to execute all the commands
