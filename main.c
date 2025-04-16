#include "minishel.h"

void	function(t_list **list, char *line)
{
	int	i;

	i = 0;

}


// ls | cat -e

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
		function(list, line);
		printf("the command is : %s\n", line);
	}
	return (0);
}