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

t_list	*tokenize(char	*line)
{
	int			i;
	int			start;
	int			op_len;
	int			in_quotes;
	char		*word;
	char		*operator;
	char		*temp;
	t_list		*head;
	t_list		*new_node;
	t_list		*list;
	t_type_node	type;


	head = NULL;
	list = NULL;
	new_node = NULL;
	start = 0;
	i = 0;
	in_quotes = 0;

	while (line[i])
	{
		type = get_type(&line[i]);
		if (line[i] == 34 || line[i] == 39) // " '
			in_quotes = !in_quotes;
		if (type != CMD && !in_quotes) // if we are in quotes , don't split;
		{
			if (i > start)
			{
				// add the hole command as string as a token to the linked list ;
				word = ft_substr(line, start, i - start); // i allocate memory here !!!!!!!!!
				temp = ft_strtrim(word, " ", "	"); // i allocate memory here !!!!!!!!!!
				if (ft_strlen(temp))
				{
					new_node = ft_lstnew(temp); // i allocate memory here !!!!!!!!!!!!!
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
				}
				// old place of adding the operator
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
		temp = ft_strtrim(word, " ", "	");
		if (ft_strlen(temp))
		{
			new_node = ft_lstnew(temp);
			new_node->type = CMD;
			if (!head)
				head = new_node;
			else
			{
				list->next = new_node;
				// list->next->next = NULL; // terminate the linked list ;
			}
		}
	}
	return (head);
}

/* i think i will work recursive descent algo , 
 first i will look for the lowest precedence operator in this order
 || , && , | , < > , >> << .
 then i will split by it , to get two nodes , the left node will be a leaf then the 
 i will call back , a recursion , to do the same work again .

*/


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

void	print_queue(t_queue *queue)
{
	while (queue)
	{
		printf("%s  ", queue->content);
		queue = queue->next;
	}
	printf("\n");
}

int main()
{
	char	*line;
	t_list	*list;
	t_queue	*queue;

	// add_history(line);
	while (1)
	{
		// printf("the len of : %zu\n", ft_strlen(""));
		line = readline("\nminishell $ ");\
		add_history(line);
		list = tokenize(line);
		// print_tokens(list);
		queue = build_sy_queue(list);
		print_queue(queue);
		build_tree(queue);
		// print_tree(list, 20);
	}
}

// cc -lreadline tokenize_y.c utils/libft/ft_lstnew_bonus.c utils/libft/ft_strncmp.c utils/libft/ft_substr.c utils/libft/ft_calloc.c utils/libft/ft_strlcpy.c utils/libft/ft_strlen.c utils/libft/ft_memset.c utils/libft/ft_strtrim.c build_queue_y.c utils_1_y.c utils/libft/ft_strdup.c            