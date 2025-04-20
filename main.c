#include "minishell.h"


int	main(int ac, char **av, char **env)
{
	t_list	*list;
	t_tree	*tree;
	char	*line;
	
	(void)ac;
	(void)av;
	(void)env;
	list = NULL;
	tree = NULL;
	while (1)
	{
		line = readline("minishell-1.0 $");
		add_history(line);
		if (is_syntax_error(line))
		{
			printf("skipping\n");
			continue ;
		}
		
		// list = tokenize(line);
		// tree = create_tree(list);
		(void)tree;
	}
	
	return (0);
}
