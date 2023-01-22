/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:32:55 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/23 02:56:58 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
//# include "../inc/colors.h"
//# include "../inc/get_next_line.h"
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <dirent.h>
# include <stdbool.h>
# include <sys/ioctl.h>
# include <dirent.h>
# include <errno.h>
# include <err.h>

typedef struct s_command
{
	char	*raw_command;
	char	**handled_cmd;
	int		p[2];
	int		infile;
	int		outfile;
}			t_command;

typedef struct s_x
{
	int			error_code;
	int			redirect_error;
	int			cmd_count;
	char		**envp;
	char		**export;
	char		*prompt;
	t_command	*cmds;
}			t_x;

typedef struct s_export_vars
{
	int		i;
	char	*tmp;
	char	*env_name;
}		t_export_vars;

extern t_x	*g_x;

void	ft_exit(char **command);
void	the_quotes(char *current_quote, char *s, int i);
bool	ft_isspace(char c);
bool	is_metachar(char c);
void	skip_spaces(char *s, int *i);
void	skip_redirection(char *s, int *i);
void	skip_quote(char *s, int *i);
int		ft_command_count(char *str);
void	seperate_command(char *s);
void	handle_command_execution(int i, bool is_in_fork);
bool	expand_single(char *s, int *i, char **dst);
char	*dollar(char *s, int *i);
char	*quote(char *s, int *i);
void	print_list(char **list);
char	*double_quote(char *s, int *i);
void	redirect(int i);
char	*redirect_path(char *s, int *i);
int		redirect_input(char *str, int *i);
int		redirect_output(char *str, int *i, bool is_append);
int		heredoc(char *str, int *i);
int		ft_change_dir(char *path);
void	handle_line(char *str);
void	handle_crtl_d(void);
void	handle_signal(int signum);
void	print_error(char *shell, char *cmd, char*str);
char	**extract_command(char *str);
char	**extract_commands(char *s);
void	handle_command(char **command, int outfd, int infd, bool is_in_fork);
void	mini_echo(char **parse, int fd);
int		ft_strcmp(const char *s1, const char *s2);
int		mini_pwd(int fd);
void	mini_env(int fd);
void	mini_pathed(char **command, int outfile, int infile);
int		ft_str2len(char **str2);
int		ft_word_count(char *str);
void	skip_spaces(char *s, int *i);
char	*ft_strdup(const char *s1);
char	*get_user(void);
char	*get_path(void);
void	get_prompt(void);
void	ft_export(char **parse, int fd);
char	**export_sorter(char **envp);
char	**storage_and_copy(char **env);
char	*env_name_getter(char *var);
char	*env_getter(char *str);
int		equal_finder(char *var);
int		export_add_var_body(t_export_vars *exp, char *var);
void	export_add_variable(char *var);
void	export_display(int i, int fd);
void	export_ctrl_and_add_var(char **parse);
char	**ft_rrealloc(char **ptr, size_t size);
int		export_add_var_helper(t_export_vars *exp, char *var);
int		export_add_var_helper2(t_export_vars *exp, char *var);
char	*strjoin_free(char *a, char *b);
char	**ft_split_free(char *str, char c);
char	*ft_substr_free(char *str, int start, int end);
int		export_add_var_body_helper(t_export_vars *exp, char *var);
void	export_add_var_equal(t_export_vars *exp, char *var);
void	ft_free(char **str);
int		unset_env_helper(int x);
int		unset_export_helper(int x);
void	unset_error(char **parse, int i);
void	ft_unset(char **parse);
void	unset_export(char **parse, char **name, int i);
void	unset_env(char **parse, char **name, int i);

#endif
