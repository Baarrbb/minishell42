/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 00:15:10 by marvin            #+#    #+#             */
/*   Updated: 2024/03/24 00:33:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*add_quotes(char *new, char *add)
{
	int		j;
	char	*new_var;

	j = 0;
	while (add[j] != '=')
		j++;
	new = strjoin(new, ft_substr(add, 0, j + 1));
	new = strjoin(new, "\"");
	new_var = ft_substr(add, j + 1, ft_strlen(add) - j);
	new = strjoin(new, new_var);
	free(new_var);
	new = strjoin(new, "\"");
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

static char **sort_env(char **env, int size)
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
