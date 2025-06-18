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
		return ;
	free_tree(tree->left);
	free_tree(tree->right);
	if (tree->args)
		free_matrix(tree->args);
	if (tree->red.outfile)
		free(tree->red.outfile);
	close_read_fd(tree);
	free(tree);
}

void	free_env(char **env, t_list **envl)
{
	ft_lstclear(envl, free);
	if (env)
		free_matrix(env);
}
