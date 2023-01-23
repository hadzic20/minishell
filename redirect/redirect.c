/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:09:00 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 14:07:37 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redirect(int cmd_index)
{
	char	*cmd;
	int		i;

	cmd = g_x->cmds[cmd_index].raw_command;
	g_x->cmds[cmd_index].infile = 0;
	g_x->cmds[cmd_index].outfile = 0;
	i = 0;
	g_x->redirect_error = 0;
	while (cmd[i] != '\0' && g_x->redirect_error == 0)
	{
		if (cmd[i] == '\"' || cmd[i] == '\'')
			skip_quote(cmd, &i);
		if (cmd[i] == '\0')
			return ;
		if (cmd[i] == '>' && cmd[i + 1] == '>')
			g_x->cmds[cmd_index].outfile = redirect_output(cmd, &i, true);
		else if (cmd[i] == '>')
			g_x->cmds[cmd_index].outfile = redirect_output(cmd, &i, false);
		else if (cmd[i] == '<' && cmd[i + 1] == '<')
			g_x->cmds[cmd_index].infile = heredoc(cmd, &i);
		else if (cmd[i] == '<')
			g_x->cmds[cmd_index].infile = redirect_input(cmd, &i);
		if (cmd[i] != '\0')
			i++;
	}
}

int	redirect_input(char *str, int *i)
{
	char	*path;
	int		file;

	(*i) += 1;
	path = expand_redirect(str, i);
	if (!path)
		return (-1);
	file = open(path, O_RDONLY, 0777);
	if (file == -1)
	{
		perror("error opening file");
		g_x->redirect_error = 1;
		return (-1);
	}
	return (file);
}

int	redirect_output(char *str, int *i, bool is_append)
{
	char	*path;
	int		file;

	if (is_append)
		(*i) += 2;
	else
		(*i) += 1;
	path = expand_redirect(str, i);
	if (!path)
		return (-1);
	if (is_append)
		file = open(path, O_CREAT | O_RDWR | O_APPEND, 0777);
	else
		file = open(path, O_CREAT | O_RDWR | O_TRUNC, 0777);
	free(path);
	if (file == -1)
	{
		perror("error opening file");
		g_x->redirect_error = 1;
		return (-1);
	}
	return (file);
}
