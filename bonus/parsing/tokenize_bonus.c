/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:53:38 by ycharkou          #+#    #+#             */
/*   Updated: 2025/06/22 17:40:19 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing_bonus.h"

t_type_node	get_type(char *content)
{
	if (ft_strncmp(content, ">>", 2) == 0)
		return (APPEND);
	else if (ft_strncmp(content, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(content, "&&", 2) == 0)
		return (AND);
	else if (ft_strncmp(content, "||", 2) == 0) 
		return (OR);
	else if (ft_strncmp(content, ">", 1) == 0)
		return (REDIRECTION_OUT);
	else if (ft_strncmp(content, "<", 1) == 0)
		return (REDIRECTION_IN);
	else if (ft_strncmp(content, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(content, "(", 1) == 0)
		return (P_OPEN);
	else if (ft_strncmp(content, ")", 1) == 0)
		return (P_CLOSE);
	else
		return (CMD);
}

int	init_variables(t_tokenize *token, t_list **head
					, t_list **last, int *in_dquotes)
{
	*head = NULL;
	*last = NULL;
	token->head = head;
	token->last = last;
	token->start = 0;
	token->i = 0;
	*in_dquotes = 0;
	return (0);
}

int	add_token(t_list **head, t_list **last, char *content, t_type_node type)
{
	t_list	*new_node;

	new_node = ft_lstnew(content);
	if (!new_node)
		return (free(content), 0);
	new_node->type = type;
	if (ft_strlen(content))
	{
		if (!*head)
		{
			*head = new_node;
			*last = new_node;
		}
		else
		{
			(*last)->next = new_node;
			*last = (*last)->next;
		}
	}
	else
		return (ft_lstclear(&new_node, free), 1);
	return (1);
}

int	extract_token(char *line, t_tokenize *token, int is_last)
{
	char	*word;
	char	*content;

	if (token->i > token->start)
	{
		word = ft_substr(line, token->start, token->i - token->start);
		content = ft_strtrim(word, " ", "\t");
		if (!content)
			return (0);
		free(word);
		if (!add_token(token->head, token->last, content, CMD))
			return (0);
	}
	if (!is_last)
	{
		content = ft_substr(line, token->i, get_operator_len(token->type));
		if (!content)
			return (0);
		if (!add_token(token->head, token->last, content, token->type))
			return (0);
		token->i += get_operator_len(token->type);
		token->start = token->i;
	}
	return (1);
}

t_list	*tokenize(char	*line)
{
	t_tokenize	token;
	t_list		*head;
	t_list		*last;
	int			in_dquotes;
	int			in_squotes;

	in_squotes = init_variables(&token, &head, &last, &in_dquotes);
	while (line[token.i])
	{
		token.type = get_type(&line[token.i]);
		handle_quotes(line[token.i], &in_squotes, &in_dquotes);
		if (token.type != CMD && !(in_squotes + in_dquotes))
		{
			if (!extract_token(line, &token, 0))
				return (ft_lstclear(&head, free), NULL);
		}
		else
			token.i++;
	}
	if (token.i > token.start)
		if (!extract_token(line, &token, 1))
			return (ft_lstclear(&head, free), NULL);
	return (head);
}
