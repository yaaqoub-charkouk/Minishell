#include "minishell.h"

#define SKY_BLUE "\033[38;5;39m"
#define RESET_COLOR "\033[0m"
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
 int 	g_sig;

// void	ll()
// {
// 	system("leaks minishell");
// }


void	pad_redirections_with_cmd(t_list **lst)
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
void	free_tokens(t_list *lst)
{
	t_list	*temp;

	if (!lst)
		return ;
	while (lst)
	{
		temp = lst->next;
		free(lst->content);
		lst->content = NULL;
		free(lst);
		lst = NULL;
		lst = temp;
	}
}

void	free_env(char **env, t_list **envl)
{
	int		i;

	ft_lstclear(envl, free);
	if (env)
	{
		i = 0;
		while (env[i])
		{
			free(env[i]);
			env[i++] = NULL;
		}
		free(env);
		env = NULL;
	}
}

void    free_tree(t_tree *tree)
{
    if (!tree)
        return;

    // Recursively free the left and right children first
    free_tree(tree->left);
    free_tree(tree->right);

    // Free the content of the current node

    if (tree->args)
        free_matrix(tree->args);
    if (tree->red.outfile)
        free(tree->red.outfile);
    // if (tree->red.file_name)
    //     free(tree->red.file_name);

    // Finally, free the node itself
    free(tree);
}

int	main(int ac, char **av, char **envp)
{
	t_list	*tokens;
	t_tree	*tree;
	t_list	*env;
	t_data	data;
	char	*line;
	char	*prompt;
	int syntax;

	(void)ac;
	(void)av;
	env = NULL;
	copy_env(envp, &env);
	tokens = NULL;
	tree = NULL;
	rl_catch_signals = 0;
	setup_signals();
	data.exit_status = 0;
	data.env = env_struct_to_char(env);
    data.envl = &env;
	syntax = 0;
	while (1)
	{
		// char cmd[128];
		// snprintf(cmd, sizeof(cmd), "lsof -p %d", getpid());
		// system(cmd);



		if (!isatty(STDIN_FILENO))
		{
			rl_instream = stdin;
			rl_outstream = fopen("/dev/null", "w");
			line = readline(NULL);
			if (rl_outstream) fclose(rl_outstream);
				rl_outstream = stdout;
		}
		else
		{
            if (data.exit_status == 0)
                prompt = GREEN " ↪ " SKY_BLUE"minishell-2.0$ " RESET_COLOR;
            else
                prompt = RED " ↪ " SKY_BLUE"minishell-2.0$ " RESET_COLOR;
            line = readline(prompt);
		}
		if (data.env)
		{
			free_env(data.env, NULL);
			data.env = env_struct_to_char(env);
		}
		data.read_fd = STDIN_FILENO;
		data.is_heredoc = 0;
		if (!line)
		{
			data.exit_status = 0;
			printf("exit\n");
			break;
		}
		
		add_history(line);
		tokens = tokenize(line); // allocate memory
		syntax = is_syntax_error(line, tokens); // free all inside except line, tokens;
		
		free(line);
		line = NULL;
		
		if (syntax)
		{
			// free tokens;
			data.exit_status = 258;
			if(syntax == 1337)
				data.exit_status = 0;
			ft_lstclear(&tokens, free);
			continue ;
		}

		pad_redirections_with_cmd(&tokens);
		// print_list(tokens);
		tree = build_tree(tokens, &data);

		// NO LEAKS UNTIL HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NO LEAKS !

		pre_execution(tree, &data);
		// print_tree(tree, 0);
		data.exit_status = execution(tree, &data, 0);
		free_tree(tree);
        tree = NULL;
		reset_terminal_mode();
		if (!isatty(STDIN_FILENO))
		{
			break ;
		}

	}
	free_env(data.env, data.envl);
	rl_clear_history();
	// while (1);
	return (data.exit_status);
}
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
