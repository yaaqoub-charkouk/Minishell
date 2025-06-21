/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akharkho <akharkho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 21:50:35 by akharkho          #+#    #+#             */
/*   Updated: 2025/06/21 20:36:17 by akharkho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	data->signaled = 0;
	pre_execution(tree, data);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, sig_quit_child);
	data->exit_status = execution(tree, data, 0);
	data->signaled = 0;
	setup_signals();
	free_tree(tree);
	tree = NULL;
	reset_terminal_mode();
	g_sig = 0;
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

void	check_env(t_data *data, t_list **env, char **envp)
{
	char	cwd[PATH_MAX];
	char	*join;
	int		val;
	char	*num;

	if (!*envp)
	{
		join = ft_strjoin("PWD=", getcwd(cwd, sizeof(cwd)), 0);
		export_process_arg(join, env, NULL);
		free(join);
		export_process_arg("_=/usr/bin/env", env, NULL);
		export_process_arg("SHLVL=1", env, NULL);
		export_process_arg("PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.",
			env, NULL);
	}
	if (getenv("SHLVL"))
		val = ft_atoi(getenv("SHLVL"), &val) + 1;
	else
		val = 1;
	copy_env(envp, env);
	data->envl = env;
	num = ft_itoa(val);
	join = ft_strjoin("SHLVL=", num, 2);
	export_process_arg(join, env, NULL);
	free(join);
}

void	initialise_vars(t_data *data, t_list **env,
	t_list **tokens, char **envp)
{
	*tokens = NULL;
	*env = NULL;
	check_env(data, env, envp);
	rl_catch_signals = 0;
	data->exit_status = 0;
	data->env = env_struct_to_char(*env);
	setup_signals();
}
