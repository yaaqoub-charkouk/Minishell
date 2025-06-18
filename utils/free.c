#include "utils.h"

void	free_matrix(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}

void	free_tree(t_tree *tree)
{
	if (!tree)
		return;

	// Recursively free the left and right children first
	free_tree(tree->left);
	free_tree(tree->right);

	// Free the content of the current node

	if (tree->args)
		free_matrix(tree->args);
	if (tree->red.outfile)
		free(tree->red.outfile);
	// if (tree->red.file_name)
	//     free(tree->red.file_name);
	close_read_fd(tree);
	// Finally, free the node itself
	free(tree);
}