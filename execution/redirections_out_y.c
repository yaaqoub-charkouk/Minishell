#include "execution.h"

// int	execute_red_out(t_tree *node, char **env, t_env **envl)
// {
// 	int	fd;
// 	int	pid;
// 	static int flag = 1;

// 	// if (node->right->type == REDIRECTION_OUT)
// 	if (node->right && node->right->type == REDIRECTION_OUT)
// 	{
// 		fd = open(node->right->left->args[0], O_TRUNC  | O_CREAT | O_RDWR, 0777);
// 		close(fd);
// 		return (execute_red_out(node->right, env, envl));
// 	}
// 	fd = open(node->args[0], O_TRUNC  | O_CREAT | O_RDWR, 0777);
// 	pid = fork();
// 	if (pid == 0 && flag)
// 	{
// 		// printf("%d\n", fd);
// 		printf("node : %s\n", node->left->args[0]);
// 		dup2(fd, 1);
// 		close(fd);
// 		// printf("%d\n", fd);
// 		flag--;
// 		exit(execute_cmd(node->left, env, envl));
// 	}
// 	waitpid(pid, NULL, 0);// need to exit with the exit status returned by the child process 
// 	close(fd);
// 	return (0);
// }


int	execute_red_out(t_tree *node, char **env, t_env **envl)
{
	int	fd;
	int	pid;

	// Traverse to the last redirection to get final output file
	while (node->right && node->right->type == REDIRECTION_OUT)
	{
		fd = open(node->right->left->args[0], O_CREAT | O_TRUNC | O_RDWR, 0777);
		if (fd < 0)
			return (perror("open"), 1);
		close(fd);
		node = node->right;
	}

	// Now node->left is the command node, and node->args[0] is the final output file
	fd = open(node->args[0], O_CREAT | O_TRUNC | O_RDWR, 0777);
	if (fd < 0)
		return (perror("open"), 1);

	pid = fork();
	if (pid < 0)
		return (perror("fork"), close(fd), 1);

	if (pid == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		exit(execute_cmd(node->left, env, envl));
	}
	close(fd);
	waitpid(pid, NULL, 0);
	return (0);
}