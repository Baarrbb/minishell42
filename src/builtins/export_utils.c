/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 00:12:46 by marvin            #+#    #+#             */
/*   Updated: 2024/03/24 00:15:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size(char **env)
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

char	*get_name_var(char *var, int add)
{
	char	*name;
	int		i;

	i = 0;
	if (!add)
	{
		while (var[i] && var[i] != '=')
			i++;
	}
	else
	{
		while (var[i] && var[i] != '+')
			i++;
	}
	name = ft_substr(var, 0, i);
	return (name);
}

char	*get_value(char *var)
{
	int		i;
	char	*val;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	i++;
	val = ft_substr(var, i, ft_strlen(var) - i);
	return (val);
}

int	cmp_name(char *var1, char *var2)
{
	char	*name;
	char	*name_in;
	int		len;

	name = get_name_var(var1, 0);
	name_in = get_name_var(var2, 0);
	if (ft_strlen(name) < ft_strlen(name_in))
		len = ft_strlen(name_in);
	else
		len = ft_strlen(name);
	return (ft_strncmp(name, name_in, len));
}
