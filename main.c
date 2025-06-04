#include "minishell.h"
 int 	g_sig;

// void	ll()
// {
// 	system("leaks minishell");
// }

#define SKY_BLUE "\033[38;5;39m"
#define RESET_COLOR "\033[0m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"

void	function(t_list **lst)
{
	t_list	*node;
	t_list	*temp;

	if (is_redirection((*lst)->type))
	{
		node = ft_lstnew(NULL);
		node->type = CMD;
		node->next = *lst;
		*lst = node;
	}
	temp = *lst;
	while (temp)
	{
		if (is_operator(temp->type) && is_redirection(temp->next->type))
		{
			node = ft_lstnew(NULL);
			node->type = CMD;
			node->next = temp->next;
			temp->next = node;
		}
		temp = temp->next;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_list	*tokens;
	t_tree	*tree;
	t_env	*env;
	t_data	data;
	char	*line;
	char *prompt;
	(void)ac;
	(void)av;
	env = NULL;
	copy_env(envp, &env);
	tokens = NULL;
	tree = NULL;
	rl_catch_signals = 0;
	setup_signals();
	data.exit_status = 0;
	while (1)
	{
		if (!isatty(STDIN_FILENO))
		{
			readline("");
		}
		else
		{
		if (data.exit_status == 0)
			prompt = GREEN " ↪ " SKY_BLUE"minishell-2.0$ " RESET_COLOR;
		else
			prompt = RED " ↪ " SKY_BLUE"minishell-2.0$ " RESET_COLOR;
		line = readline(prompt);
		}
		data.env = env_struct_to_char(env);
		data.envl = &env;
		data.read_fd = STDIN_FILENO;
		data.done_with_heredoc = 0;
		if (!line)
		{
			printf("line is NULL from readline\n");
			data.exit_status = 0;
			printf("exit\n");
			break;
		}
		
		add_history(line);
		tokens = tokenize(line);
		int syntax;
		syntax = is_syntax_error(line, tokens);
		if (syntax)
		{
			printf("skipping\n");    // need to free tokens
			data.exit_status = 258;
			continue ;
		}
		function(&tokens);
		tree = build_tree(tokens, &data); // free queue , op stack , tokens
		// here_doc(tree, &data); // prepare all heredoc first
		
		// print_tokens(tokens);

		pre_execution(tree, &data);
		print_tree(tree, 0);
		data.exit_status = execution(tree, &data, 0);
		if (!isatty(STDIN_FILENO))
		{
			break ;
		}
		// (void)tree;
	}
	rl_clear_history();
	return (data.exit_status);
}
// minishell-2.0$ /ls
// echo "$(ls)" bash: command substitution
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
// export a="ls -la" $a -> execute ls -la
// minishell-1.9$ "" -> command not found
// echo -nnnnnnnn hello
// some commands does not write / reopen on it's associated outfile
// < out cat > out // what happens


// ls > a | < b ls > c
// ls > out | < out ls > out7 && < out cat << here | grep HOME > out
// syntax error "      " segfault;
// cat << a > output done
// CTR C for here_doc

// ls | cat > a < main.c
// execve format error ??????
//  > out && ls
