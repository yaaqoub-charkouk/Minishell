#include "execution.h"

char    *open_return_infile(t_tree  *node, t_data *data, t_list **args_list)
{
	int fd;
	
	if (!node->right)
	{
		add_cmd_options(args_list, node->args, 1);
		return (node->args[0]);
	}
	if (node->right)
	{
		fd = open(node->left->args[0], O_RDONLY, 0777);
		if (fd < 0)
		{
			perror(node->left->args[0]);
			exit (1);
		}
		add_cmd_options(args_list, node->left->args, 1);
		close(fd);
		return (open_return_infile(node->right, data, args_list));
	}
	return (node->right->args[0]);
}

int	execute_red_in(t_tree *node, t_data *data)
{
	// int fd;
	// int pid;
	char    **arguments;
	char    *infile;
	t_list  *args_list;
	
	args_list = NULL;
	arguments = NULL;

	if (node->left)
	{
		arguments = node->left->args;
		args_list = add_cmd_options(&args_list, arguments, 0);
	}

	infile = open_return_infile(node->right, data, &args_list);
	printf("infile : %s\n", infile);

	arguments = list_to_char(args_list);
	node->args = arguments; // need to free old args;


	// just redirect the command input to infile , there are several edge cases 
	// don't code heredoc we will handle some cases together , it was hard a sat;


	// fd = open(infile, O_RDONLY, 0777);
	// if (fd < 0)
	// 	perror("red_in open");
	// redirect the input here;
	return (0);
}