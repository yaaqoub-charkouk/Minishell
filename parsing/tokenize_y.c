#include "parsing.h"

t_type_node get_type(char *content)
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

int	get_operator_len(t_type_node type)
{
	if (type == APPEND || type == HEREDOC || type == AND || type == OR)
		return (2);
	return (1);
}

void	add_token(t_list **head, t_list **last, char *token, t_type_node type)
{
	t_list	*new_node;

	new_node = ft_lstnew(token);
	if (!new_node)
		return ;
	new_node->type = type;
	if (ft_strlen(token))
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
}
int	init_variables(t_tokenize *token, t_list **head, t_list **last, int *in_dquotes)
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

void	extract_token(char *line, t_tokenize *token, int is_last)
{
	char	*word;
	char	*content;

	if (token->i > token->start)
	{
		word = ft_substr(line, token->start, token->i - token->start);
		content = ft_strtrim(word, " ", "	");
		free(word);
		add_token(token->head, token->last, content, CMD);
	}
	if (!is_last)
	{
		content = ft_substr(line, token->i, get_operator_len(token->type));
		add_token(token->head, token->last, content, token->type);
		token->i += get_operator_len(token->type);
		token->start = token->i;
	}
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
		if (line[token.i] == 34 && !in_squotes)
			in_dquotes = !in_dquotes;
		if (line[token.i] == 39 && !in_dquotes)
			in_squotes = !in_squotes;
		if (token.type != CMD && !(in_squotes + in_dquotes))
			extract_token(line, &token, 0);
		else
			token.i++;
	}
	if (token.i > token.start)
		extract_token(line, &token, 1);
	return (head);
}

void print_tokens(t_list *tokens)
{
	while (tokens)
	{
		printf("TOKEN: %-10s \n", tokens->content);
		switch (tokens->type)
		{
			case CMD:
				printf("  TYPE: CMD\n");
				break;
			case PIPE:
				printf("  TYPE: PIPE\n");
				break;
			case REDIRECTION_IN:
				printf("  TYPE: REDIRECTION_IN\n");
				break;
			case REDIRECTION_OUT:
				printf("  TYPE: REDIRECTION_OUT\n");
				break;
			case APPEND:
				printf("  TYPE: APPEND\n");
				break;
			case HEREDOC:
				printf("  TYPE: HEREDOC\n");
				break;
			case AND:
				printf("  TYPE: AND\n");
				break;
			case OR:
				printf("  TYPE: OR\n");
				break;
			case P_OPEN:
				printf("  TYPE: (\n");
				break;
			case P_CLOSE:
				printf("  TYPE: )\n");
				break;
			default:
				printf("  TYPE: Unknown\n");
				break;
		}
		tokens = tokens->next;
	}
}



// cc -lreadline tokenize_y.c utils/libft/ft_lstnew_bonus.c utils/libft/ft_strncmp.c utils/libft/ft_substr.c utils/libft/ft_calloc.c utils/libft/ft_strlcpy.c utils/libft/ft_strlen.c utils/libft/ft_memset.c utils/libft/ft_strtrim.c build_queue_y.c utils_1_y.c utils/libft/ft_strdup.c            