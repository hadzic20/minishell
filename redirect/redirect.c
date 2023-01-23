/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:09:00 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 16:25:04 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
 * Just sets the cmd.infile and cmd.outfile fields.
 * 
 * Only if it sees any errors, it sets the g_x->error_code
 * field.
 * Returns false on any error
 * TODO: See if you can remove that return(true) line
 */
bool	redirect(t_command *cmd)
{
	int		i;

	cmd->infile = 0;
	cmd->outfile = 1;
	g_x->redirect_error = 0;
	i = 0;
	while (cmd->raw_command[i] != '\0' && g_x->redirect_error == 0)
	{
		if (cmd->raw_command[i] == '\"' || cmd->raw_command[i] == '\'')
			skip_quote(cmd->raw_command, &i);
		if (cmd->raw_command[i] == '\0')
			return (true);
		if (cmd->raw_command[i] == '>' && cmd->raw_command[i + 1] == '>')
			cmd->outfile = redirect_output(cmd->raw_command, &i, true);
		else if (cmd->raw_command[i] == '>')
			cmd->outfile = redirect_output(cmd->raw_command, &i, false);
		else if (cmd->raw_command[i] == '<' && cmd->raw_command[i + 1] == '<')
			cmd->infile = heredoc(cmd->raw_command, &i);
		else if (cmd->raw_command[i] == '<')
			cmd->infile = redirect_input(cmd->raw_command, &i);
		if (cmd->raw_command[i] != '\0')
			i++;
	}
	if (g_x->redirect_error != 0)
		g_x->error_code = g_x->redirect_error;
	return (g_x->redirect_error == 0);
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
	free(path);
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
