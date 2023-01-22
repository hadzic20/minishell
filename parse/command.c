/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:23:04 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:26:25 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**extract_command(char *s)
{
	int		i;
	int		j;
	char	**command;
	int		len;

	j = 0;
	if (ft_word_count(s) == 0)
		return (NULL);
	command = malloc(1 * sizeof(char *));
	command[0] = NULL;
	i = 0;
	len = 0;
	while (s[i] != '\0')
	{
		skip_spaces(s, &i);
		if (s[i] == '\0')
			break ;
		if (len <= j)
		{
			command = ft_rrealloc(command, j + 1);
			command[j] = malloc(ft_strlen(s) * sizeof(char));
			command[j][0] = '\0';
			command[j + 1] = NULL;
			len = j;
		}
		if (!expand_single(s, &i, &command[j]))
			continue ;
		if (ft_isspace(s[i]) || s[i] == '\0')
			j++;
	}
	if (command[j])
		free(command[j]);
	command[j] = NULL;
	return (command);
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
	g_x->cmds = malloc((g_x->cmd_count) * sizeof(t_command));
	while (++i < g_x->cmd_count)
		g_x->cmds[i].raw_command = ft_calloc((ft_strlen(s) + 1), sizeof(char));
	i = 0;
	while (s[i] != '\0')
	{
		k = 0;
		while (s[i] != '\0' && (s[i] != '|' || current_quote != '\0'))
		{
			if (current_quote == '\0' && (s[i] == '"' || s[i] == '\''))
				current_quote = s[i];
			else if (current_quote != '\0' && s[i] == current_quote)
				current_quote = '\0';
			g_x->cmds[j].raw_command[k++] = s[i++];
		}
		g_x->cmds[j].raw_command[k] = '\0';
		if (s[i] == '|')
			i++;
		j++;
	}
}

void	ft_exit(char **command)
{
	int	status;

	status = 0;
	if (command[0] != NULL && command[1] != NULL)
		status = ft_atoi(command[1]);
	exit(status);
}

void	handle_command(char **command, int outfd, int infd, bool is_in_fork)
{
	int	pid;
	int	status;

	if (command == NULL)
		return ;
	if (ft_strncmp(command[0], "cd", sizeof("cd")) == 0)
		ft_change_dir(command[1]);
	else if (ft_strncmp(command[0], "pwd", sizeof("pwd")) == 0)
		mini_pwd(outfd);
	else if (ft_strncmp(command[0], "env", sizeof("env")) == 0)
		mini_env(outfd);
	else if (ft_strncmp(command[0], "echo", sizeof("echo")) == 0)
		mini_echo(command, outfd);
	else if (ft_strncmp(command[0], "export", sizeof("export")) == 0)
		ft_export(command, outfd);
	else if (ft_strncmp(command[0], "unset", sizeof("unset")) == 0)
		ft_unset(command);
	else if (ft_strncmp(command[0], "exit", sizeof("exit")) == 0)
		ft_exit(command);
	else if (!is_in_fork)
	{
		pid = fork();
		if (pid == 0)
		{
			mini_pathed(command, outfd, infd);
		}
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
	handle_command(g_x->cmds[i].handled_cmd, g_x->cmds[i].outfile,
		g_x->cmds[i].infile, is_in_fork);
}
