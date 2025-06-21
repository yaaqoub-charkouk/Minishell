#include "minishell.h"

int	g_sig;

int	read_tokenize(t_data *data, t_list **tokens)
{
	char	*line;
	char	*prompt;

	if (!isatty(STDIN_FILENO))
		line = readline("");
	else
	{
		if (data->exit_status == 0)
			prompt = GREEN SKY_BLUE"minishell-2.0$ " RESET_COLOR;
		else
			prompt = RED SKY_BLUE"minishell-2.0$ " RESET_COLOR;
		line = readline(prompt);
	}
	if (!line)
	{
		data->exit_status = 0;
		ft_putstr_fd("\033[A", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();      
		ft_putstr_fd("exit\n", 2);
		return (42);
	}
	add_history(line);
	return (tokenize_syntax(data, tokens, line));
}

int	main(int ac, char **av, char **envp)
{
	t_list	*tokens;
	t_list	*env;
	t_data	data;
	int		ret;

	(void)ac;
	(void)av;
	initialise_vars(&data, &env, &tokens, envp);
	while (1)
	{
		free_env(data.env, NULL);
		data.env = env_struct_to_char(env);
		data.read_fd = STDIN_FILENO;
		data.is_heredoc = 0;
		ret = read_tokenize(&data, &tokens);
		if (ret == 42)
			break ;
		else if (ret == 258)
			continue ;
		if (build_execute(tokens, &data) == 42)
			break ;
	}
	free_env(data.env, data.envl);
	rl_clear_history();
	return (data.exit_status);
}

// multiple tabs !!!!
