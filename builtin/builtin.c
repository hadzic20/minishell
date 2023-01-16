#include "../minishell.h"

int	ft_change_dir(char *path)
{
	if (!path || (path && *path == 0) || !ft_strncmp(path, "~", 1))
	{
		if (access(getenv("HOME"), F_OK) != -1)
			chdir(getenv("HOME"));
		else
		{
			print_error("minishell", getenv("HOME"), \
						"No such a file or directory");
			return (0);
		}
	}
	else
	{
		if (access(path, F_OK) != -1)
			chdir(path);
		else
		{
			print_error("minishell", path, "No such a file or directory");
			return (0);
		}
	}
	return (1);
}

int	mini_pwd(int fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, fd);
	free(pwd);
	return (0);
}

void	mini_env(int fd)
{
	int	i;
	int	env_size;

	i = -1;
	// TODO: Use g_x->export here
	env_size = ft_str2len(g_x->envp) - 4;
	while (++i < env_size)
	{
		write(fd, g_x->envp[i], ft_strlen(g_x->envp[i]));
		write(fd, "\n", 1);
	}
	write(fd, "_=/usr/bin/env\n", 15);
}

// Memory leakler var
char	*find_path(char *name)
{
	char *path;
	char **path_to_search;
	char *str;
	int	i;
	int j;
	
	if (name[0] == '.' || name[0] == '/')
		return (name);
	path = ft_strjoin("/", name);
	i = 0;
	j = -1;
	while (++j < ft_str2len(g_x->envp) - 4)
	{
		if (ft_strnstr(g_x->envp[j], "PATH=", 5))
		{
			str = ft_substr(g_x->envp[j], 5, ft_strlen(g_x->envp[j]));
			i = 1;
		}
	}
	if (i == 0)
		return (NULL);
	path_to_search = ft_split(str, ':');
	i = -1;
	while (path_to_search[++i])
	{
		path_to_search[i] = ft_strjoin(path_to_search[i], path);
		if (access(path_to_search[i], X_OK) != -1)
		{
			return (path_to_search[i]);
		}
	}
	return (NULL);
}

void	mini_pathed(char **command, int fd)
{
	char	*path;

	path = find_path(command[0]);
	if (path == NULL)
		exit(127);
	
	if (fd > 1)
		dup2(fd, 1);
	
	execve(path, command, NULL);
	perror("execve error");
	exit(127);
}

void	mini_echo(char **parse, int fd)
{
	int	i;

	i = 1;
	if ((ft_strcmp(parse[1], "-n")) == 0) // -n varsa
	{
		while (parse[++i])
		{
			ft_putstr_fd(parse[i], fd);
			if (parse[i] && parse[i + 1])
				ft_putstr_fd(" ", fd);
		}
	}
	else if (ft_strcmp(parse[1], "-n")) // -n yoksa
	{
		i = 0;
		while (parse[++i])
		{
			ft_putstr_fd(parse[i], fd);
			if (parse[i] && parse[i + 1])
				ft_putstr_fd(" ", fd);
		}
		ft_putstr_fd("\n", fd);
	}
}
