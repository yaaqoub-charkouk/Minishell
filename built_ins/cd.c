#include "built_ins.h"

void	update_pwd(char **env, char *path, char *oldpwd)
{
	int		i;
	char	*join;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PWD=", 4) == 0)
		{
			// free(env[i]);
			join = ft_strjoin("PWD=", path);
			if (!join)
				return ;
			env[i] = join;
		}
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			// free(env[i]);
			join = ft_strjoin("OLDPWD=", oldpwd);
			if (!join)
				return ;
			env[i] = join;
		}
		i++;
	}
}

int	built_in_cd(char **args, char **envp)
{
	char	*path;
	char	*oldpwd;
	char	*prev_oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("minishell: getcwd"), 1);
	if (!args[1] || !*args[1] || args[1][0] == '~')
	{
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (free(oldpwd), 1);
		}
	}
	else if (args[1] && args[1][0] == '-')
	{
		prev_oldpwd = getenv("OLDPWD");
		if (!prev_oldpwd)
		{
			write(2, "cd: OLDPWD not set\n", 19);
			return (free(oldpwd), 1);
		}
		printf("%s\n", prev_oldpwd);
		path = prev_oldpwd;
	}
	else if (args[1][0] == '\"' || args[1][0] == '$')
	{
		char *special;
		while (*args[1] == '\"')
			args[1]++;
		special = args[1];
		while (*args[1] != '\"')
			args[1]++;
		*args[1] = '\0';
		if (!special)
			return (free(oldpwd), 1);
		if (*special == '$')
			special++;
		path = getenv(special);
		if (!path)
		{
			perror("cd :");
			return (free(oldpwd), 1);
		}
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (free(oldpwd), 1);
	}
	newpwd = getcwd(NULL, 0);
	update_pwd(envp, newpwd, oldpwd);
	return (free(oldpwd), free(newpwd), 0);
}
