#include "minishell.h"


int	main(int ac, char **av, char **env)
{
	t_list	*tokens;
	t_tree	*tree;
	char	*line;

	(void)ac;
	(void)av;
	(void)env;
	tokens = NULL;
	tree = NULL;
	while (1)
	{
		line = readline("minishell-1.3$ ");
		add_history(line);
		tokens = tokenize(line);
		if (is_syntax_error(line, tokens))
		{
			printf("skipping\n");
			continue ;
		}
		// print_tokens(tokens);
		tree = build_tree(tokens);
		(void)tree;
	}
	
	return (0);
}
