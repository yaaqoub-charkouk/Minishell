#include "minishell.h"


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
    else
        return (CMD);
}

int	get_operator_len(t_type_node type)
{
	if (type == PIPE || type == REDIRECTION_OUT || type == REDIRECTION_IN)
		return (1);
	return (2);
}

t_list    *tokenize(char	*line)
{
	int			i;
	int			start;
	int			op_len;
	char		*word;
	char		*operator;
	t_list		*head;
	t_list		*new_node;
	t_list		*list;
	t_type_node	type;

	head = NULL;
	list = NULL;
	new_node = NULL;
	start = 0;
	i = 0;

	while (line[i])
	{
		type = get_type(&line[i]);
		if (type != CMD)
		{
			if (i > start)
			{
				// add the hole command as string as a token to the linked list ;
				word = ft_substr(line, start, i - start);
				new_node = ft_lstnew(word);
				new_node->type = CMD;
				if (!head)
				{
					head = new_node;
					list = new_node;
				}
				else
				{
					list->next = new_node;
					list = list->next;
				}
				// ad the operator it self as token to the linked list ;
				// op_len = get_operator_len(type);
				// operator = ft_substr(line, i, op_len);
				// new_node = ft_lstnew(operator);
				// new_node->type = type;
				// if (!head)
				// {
				// 	head = new_node;
				// 	list = new_node;
				// }
				// else
				// {
				// 	list->next = new_node;
				// 	list = list->next;
				// }
				// i += op_len;
				// start = i;			
			}
			// ad the operator it self as token to the linked list ;
			op_len = get_operator_len(type);
			operator = ft_substr(line, i, op_len);
			new_node = ft_lstnew(operator);
			new_node->type = type;
			if (!head)
			{
				head = new_node;
				list = new_node;
			}
			else
			{
				list->next = new_node;
				list = list->next;
			}
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
		new_node = ft_lstnew(word);
		new_node->type = CMD;
		if (!head)
			head = new_node;
		else
		{
			list->next = new_node;
			// list->next->next = NULL; // terminate the linked list ;
		}
	}
	return (head);
}

void	print_tokens(t_list *tokens)
{
	while (tokens)
	{
		printf("TOKEN: %-10s\n", tokens->content);
		tokens = tokens->next;
	}
}
// cases to handle 
// < "" > out

int main()
{
	char	*line;
	t_list	*list;
	// add_history(line);
	while (1)
	{
		line = readline("minishell $ ");\
		add_history(line);
		list = tokenize(line);
		print_tokens(list);
	}
	
}
// cc -lreadline parsing_3_y.c utils/libft/ft_lstnew_bonus.c utils/libft/ft_strncmp.c utils/libft/ft_substr.c utils/libft/ft_calloc.c utils/libft/ft_strlcpy.c utils/libft/ft_strlen.c utils/libft/ft_memset.c