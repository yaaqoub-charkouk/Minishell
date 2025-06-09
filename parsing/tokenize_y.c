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

t_list	*tokenize(char	*line)
{
	int			i;
	int			start;
	int			op_len;
	int			in_dquotes;
	int			in_squotes;
	char		*word; // i free it after trim 
	char		*operator; // the content for tokens and queue
	char		*trimmed; // same as operator
	t_list		*head;
	t_list		*new_node;
	t_list		*last;
	t_type_node	type;


	head = NULL;
	last = NULL;
	new_node = NULL;
	start = 0;
	i = 0;
	in_dquotes = 0;
	in_squotes = 0;

	while (line[i])
	{
		type = get_type(&line[i]);
		if (line[i] == 34 && !in_squotes) // " '
			in_dquotes = !in_dquotes;
		if (line[i] == 39 && !in_dquotes)
			in_squotes = !in_squotes;
		
		if (type != CMD && !(in_squotes + in_dquotes)) // if we are in quotes , don't split;
		{
			if (i > start) // add the hole command as string as a token to the linked last ;
			{
				word = ft_substr(line, start, i - start); // free it after trim
				trimmed = ft_strtrim(word, " ", "	");// no need to realloc for it , free it at expand
				free(word);

				add_token(&head, &last, trimmed, CMD);

			}
			op_len = get_operator_len(type);
			operator = ft_substr(line, i, op_len); // content

			add_token(&head, &last, operator, type);

			i += op_len;
			start = i;	
		}
		else
			i++;
	}
	// add the final command as a token or if there is only the command without operators;
	if (i > start)
	{
		word = ft_substr(line, start, i - start);
		trimmed = ft_strtrim(word, " ", "	");
		free(word);

		add_token(&head, &last, trimmed, CMD);
		
	}

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