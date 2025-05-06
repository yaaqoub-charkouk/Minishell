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
	rl_catch_signals = 0;
	setup_signals();
	// signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell-1.3$ ");
		if(feof(stdin))
		{ 
			printf("EOF\n");
			break; // exit the program
		}
		if (!line)
		{
			printf("line is NULL from readline\n");
			break;
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
	rl_clear_history();
	return (0);
}
