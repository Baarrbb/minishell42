/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 20:32:20 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/24 21:56:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_already_in(char *var, char **env)
{
	char	*name;
	char	*name_in;
	int		j;
	int		len;

	if (!env)
		return (1);
	name = get_name_var(var, 0);
	if (!name)
		return (0);
	j = -1;
	while (env[++j])
	{
		name_in = get_name_var(env[j], 0);
		if (ft_strlen(name) < ft_strlen(name_in))
			len = ft_strlen(name_in);
		else
			len = ft_strlen(name);
		if (!ft_strncmp(name, name_in, len))
			return (free(name), free(name_in), 1);
		free(name_in);
	}
	free(name);
	return (0);
}

static	char	**add_var(char **tmp, char **new, char *var, int add)
{
	int		i;
	char	*val;

	i = -1;
	while (tmp[++i])
	{
		if (!cmp_name(var, tmp[i]))
		{
			if (!add)
				new[i] = ft_strdup(var);
			else
			{
				new[i] = ft_strdup(tmp[i]);
				val = get_value(var);
				new[i] = strjoin(new[i], val);
				free(val);
			}
		}
		else
			new[i] = ft_strdup(tmp[i]);
	}
	return (new);
}

void	put_var(char ***env, char *var, int add)
{
	char	**tmp;
	char	**new;
	int		i;

	if (!var)
		return ;
	tmp = *env;
	new = ft_calloc(get_size(tmp) + 2, sizeof(char *));
	if (!new)
		return ;
	i = -1;
	if (!check_already_in(var, *env))
	{
		while (tmp[++i])
			new[i] = ft_strdup(tmp[i]);
		new[i] = ft_strdup(var);
	}
	else
		new = add_var(tmp, new, var, add);
	free_char_tab(*env);
	*env = new;
}

static int	check_arg(char *arg)
{
	int		j;
	char	*err;

	err = ": not a valid identifier";
	if (arg[0] && ft_isdigit(arg[0]))
		return (printf("%s`%s'%s\n", ERROR_EXPORT, arg, err), 1);
	j = -1;
	if (arg[0] == '=')
		return (printf("%s`%s'%s\n", ERROR_EXPORT, arg, err), 1);
	while (arg[++j])
	{
		if (arg[j] == '=' && arg[j - 1] && arg[j - 1] == '+')
			return (2);
	}
	j = -1;
	while (arg[++j] && arg[j] != '=')
	{
		if (!ft_isalnum(arg[j]) && arg[j] != '_')
			return (printf("%s`%s'%s\n", ERROR_EXPORT, arg, err), 1);
	}
	if (j == 0)
		return (printf("%s`%s'%s\n", ERROR_EXPORT, arg, err), 1);
	return (0);
}

int	our_export(t_cmd *cmd, char ***env)
{
	int		i;
	char	*wo_add;
	int		ret;

	i = 0;
	ret = 0;
	while (cmd->cmd[++i])
	{
		if (check_arg(cmd->cmd[i]) == 1)
		{
			ret = 1;
			continue ;
		}
		else if (check_arg(cmd->cmd[i]) == 2)
		{
			wo_add = get_var_wo_add(cmd->cmd[i]);
			put_var(env, wo_add, 1);
			free(wo_add);
		}
		else if (cmd->var_env && !check_already_in(cmd->cmd[i], *env))
			print_export_alpha(*env, get_size(*env));
		else
			put_var(env, cmd->cmd[i], 0);
	}
	if (i == 1 && *env)
		print_export_alpha(*env, get_size(*env));
	return (ret);
}
