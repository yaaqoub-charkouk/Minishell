#include "execution.h"


// store the command to execute and build its args
// open all outfiles and return the last one 


t_list	*add_cmd_options(t_list **args_list, char **args, int i)
{
	if (!args || !*args)
		return (printf("add_cmd_options\n"), NULL); // delete printf;
	while (args[i])
	{
		ft_lstadd_back(args_list, ft_lstnew(args[i]));
		i++;
	}
	return (*args_list);
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



char	*open_return_outfile(t_tree *node, t_data *data, t_list	**args_list, t_type_node *red_type)
{
	int	fd;
	int	flag;
	
	if (!node->right)
	{
		add_cmd_options(args_list, node->args, 1);
		return (node->args[0]);
	}
	if (node->right)
	{

		flag = O_CREAT | O_RDWR;
		if (*red_type == APPEND)
			flag |= O_APPEND;
		else
			flag |= O_TRUNC;
		
		
		fd = open(node->left->args[0], flag, 0777);
		add_cmd_options(args_list, node->left->args, 1);
		close(fd);
		*red_type = node->type;
		return (open_return_outfile(node->right, data, args_list, red_type));
	}
	return (node->right->args[0]); // if right not a redirection ---> right is the outfile;
}



int	execute_red_out(t_tree *node, t_data *data)
{
	int		fd;
	int		pid;
	char	**arguments; // need to be built from all redirections args ;
	char	*outfile;
	t_list	*args_list;
	int		flag;
	t_type_node	red_type;

	red_type = node->type;
	args_list = NULL;
	arguments = NULL;

	if (node->left)
	{
		arguments = node->left->args; // those line must be interpreted once ;
		args_list = add_cmd_options(&args_list, arguments, 0); // add left / first args ;
	}

	outfile = open_return_outfile(node->right, data, &args_list, &red_type); // essential
	printf("outfile : %s\n", outfile);

	arguments = list_to_char(args_list);
	node->args = arguments;

	flag = O_CREAT | O_RDWR;
	if (red_type == APPEND)
		flag |= O_APPEND;
	else
		flag |= O_TRUNC;


	fd = open(outfile ,flag, 0777);
	if(fd < 0)
		perror("open");
	pid = fork();
	if (pid == 0)
	{
		dup2(fd, 1);
		close(fd);
		exit(execute_cmd(node, data, 1)); // kanet b 0; xof wax hadxi hiya hadik
	}
	waitpid(pid, NULL, 0); // need to exit with the exit status returned by the child process 
	close(fd);
	return (0);
}

// bash-3.2$ ls >> ""
// bash: : No such file or directory