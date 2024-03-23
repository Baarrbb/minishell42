/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 20:32:20 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/23 18:29:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_size(char **env)
{
	int		i;

	i = 0;
	if (env)
	{
		while (env[i])
			i++;
	}
	return (i);
}

static char	**cpy_env_sort(char **env)
{
	char	**sort;
	char	*new_var;
	int		size;
	int		i;
	int		j;
	int		len_var;

	if (!env)
		return (0);
	size = get_size(env);
	i = -1;
	sort = ft_calloc(size + 1, sizeof(char *));
	if (!sort)
		return (0);
	while (env[++i])
	{
		len_var = (int)ft_strlen(env[i]);
		j = 0;
		while (env[i][j] != '=')
			j++;
		new_var = ft_substr(env[i], 0, j + 1);
		sort[i] = strjoin(sort[i], new_var);
		free(new_var);
		if (ft_strchr(env[i], '='))
		{
			sort[i] = strjoin(sort[i], "\"");
			new_var = ft_substr(env[i], j + 1, len_var);
			sort[i] = strjoin(sort[i], new_var);
			free(new_var);
			sort[i] = strjoin(sort[i], "\"");
		}
	}
	return (sort);
}

static void	print_export_alpha(char **env)
{
	char	*tmp;
	int		i;
	int		j;
	char	**sort;

	sort = cpy_env_sort(env);
	i = -1;
	while (++i < get_size(env) - 1)
	{
		j = -1;
		while (++j < get_size(env) - i - 1)
		{
			if (ft_strncmp(sort[j], sort[j + 1], ft_strlen(sort[j])) > 0)
			{
				tmp = sort[j];
				sort[j] = sort[j + 1];
				sort[j + 1] = tmp;
			}
		}
	}
	i = -1;
	while (sort[++i])
	{
		if (!ft_strncmp(sort[i], "_=", 2))
			continue;
		printf("declare -x %s\n", sort[i]);
	}
	free_char_tab(sort);
}

static char *get_name_var(char *var)
{
	char	*name;
	int		i;

	while (var[i] && var[i] != '=')
		i++;
	name = ft_substr(var, 0, i);
	return (name);
}

static int	check_already_in(char *var, char **env)
{
	char	*name;
	char	*name_in;
	int		j;
	int		len;

	if (!env)
		return (1);
	name = get_name_var(var);
	j = -1;
	while (env[++j])
	{
		name_in = get_name_var(env[j]);
		if (ft_strlen(name) < ft_strlen(name_in))
			len = ft_strlen(name_in);
		else
			len = ft_strlen(name);
		if (!ft_strncmp(name, name_in, len))
			return (free(name), free(name_in), 1);
	}
	free(name);
	free(name_in);
	return (0);
}

void	put_var(char ***env, char *var)
{
	char	**tmp;
	char	**new;
	char	*name;
	char	*name_in;
	int		i;
	int		len;

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
	{
		name = get_name_var(var);
		while (tmp[++i])
		{
			name_in = get_name_var(tmp[i]);
			if (ft_strlen(name) < ft_strlen(name_in))
				len = ft_strlen(name_in);
			else
				len = ft_strlen(name);
			if (!ft_strncmp(name, name_in, len))
			{
				
			}
		}
	}

	free_char_tab(*env);
	*env = new;
}

static int	check_arg(char *arg)
{
	int	j;

	if (arg[0] && ft_isdigit(arg[0]))
	{
		printf("%s`%s': not a valid identifier\n", ERROR_EXPORT, arg);
		return (1);
	}
	j = -1;
	while (arg[++j] && arg[j] != '=')
	{
		if (!ft_isalnum(arg[j]) && arg[j] != '_')
		{
			printf("%s`%s': not a valid identifier\n", ERROR_EXPORT, arg);
			return (1);
		}
	}
	if (j == 0)
	{
		printf("%s`%s': not a valid identifier\n", ERROR_EXPORT, arg);
		return (1);
	}
	return (0);
}

void	our_export(t_cmd *cmd, char ***env)
{
	int		i;
	int		j;
	char	*arg;

	i = 0;
	while (cmd->cmd[++i])
	{
		arg = cmd->cmd[i];
		if (check_arg(arg))
			continue ;
		put_var(env, cmd->cmd[i]);
	}
	if (i == 1 && *env)
		print_export_alpha(*env);
}
