#include"../minishell.h"

// var is ahmet=mehmet
// exp->tmp is ahmet
// If this returns true, parent returns
static int	export_add_var_body(t_export_vars *exp, char *var)
{
	exp->env_name = env_name_getter(g_x->export[exp->i]);
	if (ft_strcmp(exp->env_name, exp->tmp) == 0)
	{
		if (export_add_var_body_helper(exp, var))
			return (1);
	}
	free(exp->env_name);
	return (0);
}

// ahmet=mehmet
// The set or update function
static void	export_add_variable(char *var)
{
	t_export_vars	exp;

	exp.i = -1;
	exp.tmp = env_name_getter(var);
	while (g_x->export[++exp.i])
	{
		if (export_add_var_body(&exp, var))
			return ;
	}
	free(exp.tmp);
	if (equal_finder(var) == 1)
		export_add_var_equal(&exp, var);
	else
	{
		g_x->export = ft_rrealloc(g_x->export, ft_str2len(g_x->export) + 1);
		g_x->export[exp.i] = ft_strdup(var);
		g_x->export[exp.i + 1] = 0;
		g_x->export = export_sorter(g_x->export);
	}
}

static void	export_display(int i, int fd)
{
	char	*env_name;
	char	*value;

	env_name = env_name_getter(g_x->export[i]);
	value = env_getter(g_x->export[i]);
    write(fd, "declare -x ", 11);
    ft_putstr_fd(env_name, fd);
    write(fd, "=\"", 3);
    ft_putstr_fd(value, fd);
    write(fd, "\"\n", 3);
	//printf("declare -x %s=\"%s\"\n", env_name, value);
	free(env_name);
	free(value);
}

// parse is the argv
// export a=123 b=234
static void	export_ctrl_and_add_var(char **parse)
{
	int	i;

	i = 1;
	while (parse[i])
	{
		if (parse[i][0] == '_' && (parse[i][1] == 0 || parse[i][1] == '='))
			printf("");
		else if ((parse[i][0] == '_' && parse[i][1] != 0) || \
				ft_isalpha(parse[i][0]))
			export_add_variable(parse[i]);
		else
		{
			printf("minishell: %s: `%s': not a valid identifier\n", \
					parse[0], parse[i]);
			g_x->error_code = 1;
		}
		i++;
	}
}

void	ft_export(char **parse, int fd)
{
	int	i;

	i = 0;
	if (ft_strcmp(parse[i], "export") == 0 && !parse[i + 1])
	{
		while (g_x->export[i])
		{
			if (equal_finder(g_x->export[i]) == 1)
				export_display(i, fd);
			else
				printf("declare -x %s\n", g_x->export[i]);
			i++;
		}
	}
	else if (ft_strcmp(parse[i], "export") == 0 && parse[i + 1])
		export_ctrl_and_add_var(parse);
}
