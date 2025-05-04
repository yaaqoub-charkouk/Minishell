#include "parsing.h"

t_list  *new_node_queue_list(t_queue *queue) // ,t_list *list)
{
	t_list	*node;

	node = (t_list *)malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = queue->content; // should i re alloc for the pointer or not 
	node->type = queue->type;
	node->next = NULL;
	// i need to free content here since it a return of a function so may be freeing the pointer here will be a good choice , and a good practice so yeah
	return (node);
}

int	push(t_queue *queue, t_list **list)
{
	t_list	*node;

	node = new_node_queue_list(queue);
	node->next = *list;
	*list = node;
	return (1);
}

void	print_list(t_list	*list)
{
	while (list)
	{
		printf ("%s ", list->content);
		list = list->next;
	}
	printf("\n");
}

void print_tree(t_tree *node, int level)
{
    if (!node)
        return;

    print_tree(node->right, level + 1);  // Print right subtree

    for (int i = 0; i < level; i++)
        printf("   ");  // Indentation
    printf("%s\n", node->cmd);  // Print current node

    print_tree(node->left, level + 1);  // Print left subtree
}

t_tree *new_tree_node(t_list	*token)
{
	t_tree	*tree_node;

	if (!token)
		return (NULL);
	tree_node = malloc(sizeof(t_tree));
	tree_node->args = ft_split(token->content, ' ');
	tree_node->cmd = token->content; // a copy from the first allocated token ;
	tree_node->type = token->type;
	tree_node->left = NULL;
	tree_node->right = NULL;
	return (tree_node);
}

void	ft_quotes(const char *str, int *in_quote)
{
	if (*str == '\'')
		*in_quote = !(*in_quote);
}

static int	check_sep(char c, char sep)
{
	return (c == sep);
}

static int	count_words(char const *str, char sep)
{
	int	c;

	c = 0;
	while (*str != '\0')
	{
		while (check_sep(*str, sep) && *str != '\0')
			str++;
		if (*str != '\0')
			c++;
		while (!check_sep(*str, sep) && *str != '\0')
			str++;
	}
	return (c);
}

static char	*ft_word_allocator(const char *str, char sep)
{
	int		i;
	int		j;
	int		len;
	int		in_qoute;
	char	*word;

	len = 0;
	in_qoute = 0;
	while (str[len] && (!check_sep(str[len], sep) || in_qoute))
	{
		ft_quotes(&str[len], &in_qoute);
		len++;
	}
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] != '\'')
			word[j++] = str[i];
		i++;
	}
	return (word[i] = '\0', word);
}

char	**split_string(const char *s, char c, char **string)
{
	int	i;
	int	in_quote;

	i = 0;
	in_quote = 0;
	while (*s)
	{
		while (check_sep(*s, c) && *s)
			s++;
		if (*s)
		{
			string[i] = ft_word_allocator(s, c);
			if (!string[i])
				return (free_string(string, i));
			i++;
		}
		while (*s && (!check_sep(*s, c) || in_quote))
		{
			ft_quotes(s, &in_quote);
			s++;
		}
	}
	string[i] = NULL;
	return (string);
}

char	**ft_split_pipex(char const *s, char c)
{
	char	**string;

	if (!s)
		return (NULL);
	string = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!string)
		return (NULL);
	return (split_string(s, c, string));
}
