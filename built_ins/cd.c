#include "built_ins.h"

char	*get_env_content(t_env *env, char *value)
{
	int	len;
	
	len = 0;
	if (!value || !*value)
		return (NULL);
	while (value[len] && ft_isalnum(value[len]))
	{
		len++;
	}
	while (env)
	{
		if (ft_strncmp(env->content, value, len) == 0 
			&& env->content[len] == '=')
			return (env->content + len + 1);
		env = env->next;
	}
	return (NULL);
}
void	update_pwd(char *oldpwd, char *newpwd, t_data *data)
{
	char **args;
	
	args = malloc(4 * sizeof(char *));
	if (!args)
	{
		free(oldpwd);
		free(newpwd);
		return ;
	}
	args[0] = "export";
	args[1] = ft_strjoin("PWD=", newpwd);
	args[2] = ft_strjoin("OLDPWD=", oldpwd);
	args[3] = NULL;
	if (!args[1] || !args[2])
	{
		free(args[1]);
		free(args[2]);
		free(args);
		free(oldpwd);
		free(newpwd);
		return ;
	}
	built_in_export(args, data);
	free(args[1]);
	free(args[2]);
	free(args);
}
char	*get_cd_path(char **args,  t_data *data, int *print_olpwd)
{
	char	*path;

	if (!args[1] || !*args[1] || args[1][0] == '~')
	{
		path = get_env_content(*data->envl, "HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (NULL);
		}
	}
	else if (args[1] && args[1][0] == '-')
	{
		path = get_env_content(*data->envl, "OLDPWD");
		if (!path)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			return (NULL);
		}
		else
			*print_olpwd = 1;
	}
	else
		path = args[1];
	return (path);
}
int	built_in_cd(char **args, t_data *data)
{
	char	*path;
	char	*oldpwd;
	char	*newpwd;
	int		print_oldpwd;

	print_oldpwd = 0;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("minishell: getcwd"), 1);
	path = get_cd_path(args, data, &print_oldpwd);
	if (!path)
		return (free(oldpwd), 1);
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (free(oldpwd), 1);
	}
	if (print_oldpwd)
		printf("%s\n", path);
	newpwd = getcwd(NULL, 0);
	update_pwd(oldpwd, newpwd, data);
	return (free(oldpwd), free(newpwd), 0);
}
