#include "minishell.h"


int	main(int ac, char **av, char **envp)
{
	t_list	*tokens;
	t_tree	*tree;
	t_env	*env;
	char	*line;

	(void)ac;
	(void)av;
	env = NULL;
	copy_env(envp, &env);
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
		execution(tree, envp, &env);
		(void)tree;
	}
	
	return (0);
}
