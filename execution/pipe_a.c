#include "execution.h"


int	execute_pipe(t_tree *node, t_data *data)
{
	int	fd[2];
	int	pidl;
	int	pidr;
	int	status;

	if (pipe(fd) < 0)
		return (perror("pipe"), 1);
	pidl = fork();
	if (pidl < 0)
		return (close(fd[0]), close(fd[1]), perror("fork"), 1);
	if (pidl == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (node->left->type == CMD) // base condition for pipe recursion
			exit(execution(node->left, data, 1));
		else
			exit(execution(node->left, data, 0));
	}
	pidr = fork();
	if (pidr < 0)
		return (close(fd[0]), kill(pidl, SIGKILL), close(fd[1]), perror("fork"), 1); //we may need to kill the left process
	if (pidr == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (node->right->type == CMD)
				exit(execution(node->right, data, 1));
		else
			exit(execution(node->right, data, 0));
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pidl, &status, 0);
	waitpid(pidr, &status, 0);
	// close_read_fd(node->left);
	// close_read_fd(node->right);
	return (WEXITSTATUS(status));
}
