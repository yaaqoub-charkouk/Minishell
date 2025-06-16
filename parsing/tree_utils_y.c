#include "parsing.h"

void	print_list(t_list	*list)
{
	while (list)
	{
		printf ("%s ", list->content);
		list = list->next;
	}
	printf("\n");
}

// void print_tree(t_tree *node, int level)
// {
//     if (!node)
//         return;

//     print_tree(node->right, level + 1);  // Print right subtree

//     for (int i = 0; i < level; i++)
//         printf("   ");  // Indentation
// 	if (node->args)
//    		printf("%s\n", node->args[0]);  // Print current node
// 	// else
// 	// 	printf("%s\n", NULL);
//     print_tree(node->left, level + 1);  // Print left subtree
// }
