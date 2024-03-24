/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 00:15:10 by marvin            #+#    #+#             */
/*   Updated: 2024/03/24 02:35:19 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*add_quotes(char *new, char *add)
{
	char	*name;
	char	*val;

	name = get_name_var(add, 0);
	val = get_value(add);
	new = strjoin(new, name);
	new = strjoin(new, "=");
	new = strjoin(new, "\"");
	new = strjoin(new, val);
	new = strjoin(new, "\"");
	free(name);
	free(val);
	return (new);
}

static char	**cpy_env_sort(char **env, int size)
{
	char	**sort;
	int		i;

	if (!env)
		return (0);
	i = -1;
	sort = ft_calloc(size + 1, sizeof(char *));
	if (!sort)
		return (0);
	while (env[++i])
	{
		if (!ft_strchr(env[i], '='))
			sort[i] = ft_strdup(env[i]);
		else
			sort[i] = add_quotes(sort[i], env[i]);
	}
	return (sort);
}

static char	**sort_env(char **env, int size)
{
	char	*tmp;
	int		i;
	int		j;
	char	**sort;

	sort = cpy_env_sort(env, size);
	i = -1;
	while (++i < size - 1)
	{
		j = -1;
		while (++j < size - i - 1)
		{
			if (ft_strncmp(sort[j], sort[j + 1], ft_strlen(sort[j])) > 0)
			{
				tmp = sort[j];
				sort[j] = sort[j + 1];
				sort[j + 1] = tmp;
			}
		}
	}
	return (sort);
}

void	print_export_alpha(char **env, int size)
{
	int		i;
	char	**sort;

	sort = sort_env(env, size);
	i = -1;
	while (sort[++i])
	{
		if (!ft_strncmp(sort[i], "_=", 2))
			continue ;
		printf("declare -x %s\n", sort[i]);
	}
	free_char_tab(sort);
}
