#include "../minishell.h"

void handle_line_utils(int i, int save_fd, char *str)
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

void handle_line(char *str)
{
	int i;
	int save_fd;
	int last_pid;
	int status;
	int pid;

	if (g_x->cmd_count == 0)
		return ;
	seperate_command(str);
	status = 0;
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
				handle_line_utils(i, save_fd, str);
				// Eğer bir builtinse ve hata yoksa buraya kod gelebilir...
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