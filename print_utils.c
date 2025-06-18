#include "parsing/parsing.h"

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
	if (node->args)
   		printf("%s\n", node->args[0]);  // Print current node
	// else
	// 	printf("%s\n", NULL);
    print_tree(node->left, level + 1);  // Print left subtree
}

