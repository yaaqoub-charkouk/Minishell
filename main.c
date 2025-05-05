#include "minishell.h"

void	ll()
{
	system("leaks minishell");
}

int	main(int ac, char **av, char **envp)
{
	t_list	*tokens;
	t_tree	*tree;
	t_env	*env;
	char	*line;
	// atexit(ll);

	(void)ac;
	(void)av;
	env = NULL;
	copy_env(envp, &env);
	tokens = NULL;
	tree = NULL;
	while (1)
	{
		line = readline("minishell-1.3$ ");
		if (!line)
		{
			printf("line is NULL from readline\n");
			continue;
		}
		add_history(line);
		tokens = tokenize(line);
		if (is_syntax_error(line, tokens))
		{
			printf("skipping\n"); // need to free tokens
			continue ;
		}
		tree = build_tree(tokens);
		// free queue , op stack , tokens
		execution(tree, envp, &env);
		(void)tree;
	}
	return (0);
}
