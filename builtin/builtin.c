#include "../minishell.h"
#include <errno.h>
#include <err.h>

int	ft_change_dir(char *path)
{
	g_x->error_code = 0;
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
			g_x->error_code = 1;
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
	env_size = ft_str2len(g_x->export);
	while (++i < env_size)
	{
		write(fd, g_x->export[i], ft_strlen(g_x->export[i]));
		write(fd, "\n", 1);
	}
	//write(fd, "_=/usr/bin/env\n", 15);
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
	while (++j < ft_str2len(g_x->export) - 4)
	{
		if (ft_strnstr(g_x->export[j], "PATH=", 5))
		{
			str = ft_substr(g_x->export[j], 5, ft_strlen(g_x->export[j]));
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

// Bu kesin olarak exitliyor
void	mini_pathed(char **command, int outfile, int infile)
{
	char	*path;
	int		pid;

	path = find_path(command[0]);
	if (path == NULL)
		exit(127);
	if (outfile > 1)
		dup2(outfile, 1);
	if (infile > 0)
		dup2(infile, 0);
	execve(path, command, g_x->export);
	perror("execve error");
	if (errno == 13)
		exit(126);
	exit(127);
}

void	mini_echo(char **parse, int fd)
{
	int		i;
	bool	newline;

	newline = true;
	i = 1;
	while (parse[i] && ft_strcmp(parse[i], "-n") == 0)
	{
		newline = false;
		i++;
	}
	while (parse[i])
	{
		ft_putstr_fd(parse[i], fd);
		if (parse[i] && parse[i + 1])
			ft_putstr_fd(" ", fd);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", fd);
}
