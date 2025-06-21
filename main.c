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
// execve: No such file or directory ---> bash: /ls: No such file or directory


//ls | pwd exit status should be 0 not 1 ; broken pipe ;
//  ↪ minishell-2.0$ ls || ps
// minishell: syntax error near unexpected token `|'
// bash-3.2$ ls (ls) <------ SYNTAX ERROR
// bash: syntax error near unexpected token `ls'

// minishell-1.9$ "" -> command not found

// echo "$(ls)" bash: command substitution
// minishell-2.0$ /ls
// $PWD ----> is a directory
// ls > "" && ls he has to not execute ls because empty file name :
// expand heredoc
// ------
// minishell-1.9$ cat << $PWD
// entry node : << 
// limiter /Users/ycharkou/cursus/minishell
// >/Users/ycharkou/cursus/minishell
// -------

// !! shouldn,t exit 
// export a="ls -la" $a -> execute ls -la : done
// echo -nnnnnnnn hello
// some commands does not write / reopen on it's associated outfile

// < out cat > out // what happens ???

// ls > a | < b ls > c
// ls > out | < out ls > out7 && < out cat << here | grep HOME > out
// syntax error "      " segfault;
// cat << a > output done
// CTR C for here_doc

// ls | cat > a < main.c
// execve format error ??????
//  > out && ls
