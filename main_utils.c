#include "minishell.h"

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

int	build_execute(t_list *tokens, t_data *data)
{
	t_tree	*tree;

	tree = NULL;
	pad_redirections_with_cmd(&tokens);
	tree = build_tree(tokens, data);
	pre_execution(tree, data);
	signal(SIGINT, SIG_IGN);
	data->exit_status = execution(tree, data, 0);
	setup_signals();
	free_tree(tree);
	tree = NULL;
	reset_terminal_mode();
	if (!isatty(STDIN_FILENO))
		return (42);
	return (0);
}

int	tokenize_syntax(t_data *data, t_list **tokens, char *line)
{
	int		syntax;

	*tokens = tokenize(line);
	syntax = is_syntax_error(line, *tokens);
	free(line);
	line = NULL;
	if (syntax)
	{
		data->exit_status = 258;
		if (syntax == 1337)
			data->exit_status = 0;
		ft_lstclear(tokens, free);
		return (258);
	}
	return (0);
}

int	read_tokenize(t_data *data, t_list **tokens)
{
	char	*line;
	char	*prompt;

	if (!isatty(STDIN_FILENO))
		line = readline(NULL);
	else
	{
		if (data->exit_status == 0)
			prompt = GREEN " ↪ " SKY_BLUE"minishell-2.0$ " RESET_COLOR;
		else
			prompt = RED " ↪ " SKY_BLUE"minishell-2.0$ " RESET_COLOR;
		line = readline(prompt);
	}
	if (!line)
	{
		data->exit_status = 0;
		printf("exit\n");
		return (42);
	}
	add_history(line);
	return (tokenize_syntax(data, tokens, line));
}

void	initialise_vars(t_data *data, t_list **env,
	t_list **tokens, char **envp)
{
	*tokens = NULL;
	*env = NULL;
	rl_catch_signals = 0;
	data->exit_status = 0;
	data->env = env_struct_to_char(*env);
	data->envl = env;
	copy_env(envp, env);
	setup_signals();
}
