/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:23:04 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 15:49:44 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Expand single argument
// It has to start from a char and end with
// a seperator
char	*expand_arg(char *s, int *i)
{
	char	*final;

	skip_redirections(s, i);
	if (s[*i] == '\0' || s[*i] == '|')
		return (NULL);
	final = ft_strdup("");
	while (s[*i] != '\0')
	{
		expand_single(s, i, &final);
		if (is_seperator(s[*i]))
			break ;
	}
	return (final);
}

char	**append_list(char **list, char *str)
{
	int		old_len;
	char	**new;
	int		i;

	old_len = 0;
	while (list[old_len])
		old_len++;
	new = malloc((old_len + 2) * sizeof(char *));
	i = 0;
	while (list[i] != NULL)
	{
		new[i] = ft_strdup(list[i]);
		free(list[i]);
		i++;
	}
	free(list);
	new[old_len] = str;
	new[old_len + 1] = NULL;
	return (new);
}

// Split the command to args, skip redirections
char	**extract_command(char *s)
{
	char	**args;
	int		i;
	char	*str;

	args = malloc(sizeof(char *));
	args[0] = NULL;
	i = 0;
	while (true)
	{
		skip_redirections(s, &i);
		str = expand_arg(s, &i);
		if (str == NULL)
			break ;
		args = append_list(args, str);
	}
	return (args);
}

void	seperate_command(char *s)
{
	int		i;
	int		j;
	int		k;
	char	current_quote;

	current_quote = '\0';
	i = -1;
	j = 0;
	g_x->cmds = ft_calloc(g_x->cmd_count, sizeof(t_command));
	while (++i < g_x->cmd_count)
		g_x->cmds[i].raw_command = ft_calloc((ft_strlen(s) + 1), sizeof(char));
	i = 0;
	while (s[i] != '\0')
	{
		k = 0;
		while (s[i] != '\0' && (s[i] != '|' || current_quote != '\0'))
		{
			the_quotes(&current_quote, s, i);
			g_x->cmds[j].raw_command[k++] = s[i++];
		}
		g_x->cmds[j].raw_command[k] = '\0';
		if (s[i] == '|')
			i++;
		j++;
	}
}

void	select_function(char **command, int outfd)
{
	if (ft_strncmp(command[0], "cd", 3) == 0)
		ft_change_dir(command[1]);
	else if (ft_strncmp(command[0], "pwd", 4) == 0)
		mini_pwd(outfd);
	else if (ft_strncmp(command[0], "env", 4) == 0)
		mini_env(outfd);
	else if (ft_strncmp(command[0], "echo", 5) == 0)
		mini_echo(command, outfd);
	else if (ft_strncmp(command[0], "export", 7) == 0)
		ft_export(command, outfd);
	else if (ft_strncmp(command[0], "unset", 6) == 0)
		ft_unset(command);
	else if (ft_strncmp(command[0], "exit", 5) == 0)
		ft_exit(command);
}

void	handle_command(char **command, int outfd, int infd, bool is_in_fork)
{
	int	pid;
	int	status;

	if (command == NULL)
		return ;
	if (ft_strncmp(command[0], "cd", 3) == 0
		|| ft_strncmp(command[0], "pwd", 4) == 0
		|| ft_strncmp(command[0], "env", 4) == 0
		|| ft_strncmp(command[0], "echo", 5) == 0
		|| ft_strncmp(command[0], "export", 7) == 0
		|| ft_strncmp(command[0], "unset", 6) == 0
		|| ft_strncmp(command[0], "exit", 5) == 0)
		select_function(command, outfd);
	else if (!is_in_fork)
	{
		pid = fork();
		if (pid == 0)
			mini_pathed(command, outfd, infd);
		status = 0;
		waitpid(pid, &status, 0);
		g_x->error_code = WEXITSTATUS(status);
	}
	else
		mini_pathed(command, outfd, infd);
}

void	handle_command_execution(int i, bool is_in_fork)
{
	g_x->cmds[i].handled_cmd = extract_command(g_x->cmds[i].raw_command);
	g_x->error_code = 0;
	if (g_x->cmds[i].handled_cmd[0] == NULL)
		return ;
	handle_command(g_x->cmds[i].handled_cmd, g_x->cmds[i].outfile,
		g_x->cmds[i].infile, is_in_fork);
}
