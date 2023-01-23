/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:22:04 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 15:11:55 by ykimirti         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_line_utils(int i, int save_fd)
{
	if (i != 0)
	{
		dup2(save_fd, 0);
		close(save_fd);
	}
	close(g_x->cmds[i].p[0]);
	if (i != g_x->cmd_count - 1)
		dup2(g_x->cmds[i].p[1], 1);
	close(g_x->cmds[i].p[1]);
	handle_command_execution(i, true);
}

void	handle_line(char *str)
{
	int	i;
	int	save_fd;
	int	last_pid;
	int	status;
	int	pid;

	if (g_x->cmd_count == 0)
		return ;
	status = 0;
	seperate_command(str);
	if (g_x->cmd_count == 1)
	{
		redirect(0);
		if (g_x->redirect_error == 0)
			handle_command_execution(0, false);
		else
			g_x->error_code = g_x->redirect_error;
	}
	else
	{
		i = -1;
		while (++i != g_x->cmd_count && g_x->cmd_count != 1)
		{
			redirect(i);
			if (g_x->redirect_error != 0)
			{
				g_x->error_code = g_x->redirect_error;
				return ;
			}
			g_x->cmds[i].outfile = 1;
			pipe(g_x->cmds[i].p);
			pid = fork();
			last_pid = pid;
			if (pid == 0)
			{
				handle_line_utils(i, save_fd);
				exit(g_x->error_code);
			}
			close(g_x->cmds[i].p[1]);
			if (i != 0)
				close(save_fd);
			save_fd = g_x->cmds[i].p[0];
		}
		close(save_fd);
		status = 0;
		waitpid(last_pid, &status, 0);
		g_x->error_code = WEXITSTATUS(status);
	}
	while (waitpid(-1, &status, 0) != -1)
		;
}

// Expand quote, dquote, dollar or word UNTIL it sees a
// ft_isspace, 
void	expand_single(char *s, int *i, char **dst)
{
	int		k;

	if (s[*i] == '"')
		*dst = strjoin_free(*dst, double_quote(s, i));
	else if (s[*i] == '\'')
		*dst = strjoin_free(*dst, quote(s, i));
	else if (s[*i] == '$')
		*dst = strjoin_free(*dst, dollar(s, i));
	else
	{
		k = ft_strlen(*dst);
		*dst = strjoin_free(*dst, ft_strdup(s));
		while (s[*i] != '\0' && !is_metachar(s[*i]) && !ft_isspace(s[*i]))
			(*dst)[k++] = s[(*i)++];
		(*dst)[k] = '\0';
	}
}
