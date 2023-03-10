/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amillahadzic <amillahadzic@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 20:12:25 by amillahadzi       #+#    #+#             */
/*   Updated: 2023/01/22 20:12:27 by amillahadzi      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../minishell.h"

void	export_add_var_equal(t_export_vars *exp, char *var)
{
	g_x->export = ft_rrealloc(g_x->export, ft_str2len(g_x->export) + 2);
	g_x->export[exp->i] = ft_strdup(var);
	g_x->export[exp->i + 1] = 0;
	g_x->export = export_sorter(g_x->export);
}

int	export_add_var_body_helper(t_export_vars *exp, char *var)
{
	if (equal_finder(g_x->export[exp->i]) == 1 && equal_finder(var) == 0)
	{
		free(exp->env_name);
		free(exp->tmp);
		return (1);
	}
	else if (equal_finder(g_x->export[exp->i]) == 0 \
			&& equal_finder(var) == 1)
	{
		if (export_add_var_helper(exp, var))
			return (1);
	}
	else if (equal_finder(g_x->export[exp->i]) == 1 \
			&& equal_finder(var) == 1)
	{
		if (export_add_var_helper(exp, var))
			return (1);
	}
	else
	{
		free(exp->env_name);
		free(exp->tmp);
		return (1);
	}
	return (0);
}

void	exprt_addvar_mini_help(t_export_vars *exp, int *env_ctrl, \
									int *x, char *var)
{
	while (g_x->envp[++(*x)])
	{
		exp->env_name = env_name_getter(g_x->envp[*x]);
		if (ft_strcmp(exp->env_name, exp->tmp) == 0)
		{
			free(exp->env_name);
			free(g_x->envp[*x]);
			*env_ctrl = 1;
			g_x->envp[*x] = ft_strdup(var);
			break ;
		}
		free(exp->env_name);
	}
}

int	export_add_var_helper(t_export_vars *exp, char *var)
{
	free(exp->env_name);
	free(exp->tmp);
	free(g_x->export[exp->i]);
	g_x->export[exp->i] = ft_strdup(var);
	g_x->export = export_sorter(g_x->export);
	return (1);
}
