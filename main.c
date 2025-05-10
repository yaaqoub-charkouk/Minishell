#include "minishell.h"

void	ll()
{
	system("leaks minishell");
}

#define SKY_BLUE "\033[38;5;39m"
#define RESET_COLOR "\033[0m"

int	main(int ac, char **av, char **envp)
{
	t_list	*tokens;
	t_tree	*tree;
	t_env	*env;
	t_data	data;
	char	*line;
	(void)ac;
	(void)av;
	env = NULL;
	copy_env(envp, &env);
	tokens = NULL;
	tree = NULL;
  
	rl_catch_signals = 0;
	setup_signals();
  
	while (1)
	{
		line = readline(SKY_BLUE"minishell-1.7$ "RESET_COLOR);
		if (!line)
		{
			printf("line is NULL from readline\n");
			break;
		}
		add_history(line);
		tokens = tokenize(line);
		if (is_syntax_error(line, tokens))
		{
			printf("skipping\n");    // need to free tokens
			continue ;
		}
		tree = build_tree(tokens); // free queue , op stack , tokens
		data.env = env_struct_to_char(env);
		data.envl = &env;
		data.read_fd = STDIN_FILENO;
		execution(tree, &data, 0);//>> free the env
		(void)tree;
	}
	rl_clear_history();
	return (0);
}

// heredoc with pipes
// cat << a > output 
//  >> h>> k>> l (SEGV)
// CTR C for here_doc
