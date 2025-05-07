#include "execution.h"


// store the command to execute and build its args
// open all outfiles and return the last one 


t_list	*add_cmd_options(t_list **args_list, char **args, int i)
{
	while (args[i])
	{
		ft_lstadd_back(args_list, ft_lstnew(args[i]));
		i++;
	}
	return (*args_list);
}



char	*open_return_outfile(t_tree *node, char **env, t_env **envl, t_list	**args_list)
{
	int	fd;

	if (!node->right)
		return (node->args[0]);
	if (node->right )
	{
		fd = open(node->left->args[0], O_TRUNC  | O_CREAT | O_RDWR, 0777);
		add_cmd_options(args_list, node->left->args, 1);
		close(fd);
		return (open_return_outfile(node->right, env, envl, args_list));
	}
	return (node->right->args[0]); // if right not a redirection ---> right is the outfile;
}


char	**list_to_char(t_list  *env) // the function to convert t_env struct to char ** ;
{
	int		size;
	int		i;
	char	**env_char;

	i = 0;
	size = ft_lstsize(env);
	env_char = malloc((size + 1) * sizeof(char *));
  if(!env_char)
     return (NULL);
	while (i < size)
	{
		env_char[i] = ft_strdup(env->content);
		env = env->next;
		i++;
	}
	env_char[i] = NULL;
	return (env_char); // need to be freed ;
}



int	execute_red_out(t_tree *node, char **env, t_env **envl)
{
	int	fd;
	int	pid;
	char	**arguments; // need to be built from all redirections args ;
	char	*outfile;
	t_list	*args_list;

	arguments = node->left->args; // those line must be interpreted once ;

	args_list = NULL;
	args_list = add_cmd_options(&args_list, arguments, 0);


	outfile = open_return_outfile(node->right, env, envl, &args_list); // essential
	

	arguments = list_to_char(args_list);
	node->args = arguments;

	fd = open(outfile, O_TRUNC  | O_CREAT | O_RDWR, 0777);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd, 1);
		close(fd);
		exit(execute_cmd(node, env, envl, 0));
	}
	waitpid(pid, NULL, 0); // need to exit with the exit status returned by the child process 
	close(fd);
	return (0);
}

