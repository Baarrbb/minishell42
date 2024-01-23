/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:05:39 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/23 19:23:57 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_elem(char ***arr, int index)
{
	int	i;

	i = index;
	free((*arr)[i]);
	while ((*arr)[i] != NULL)
	{
		(*arr)[i] = (*arr)[i + 1];
		i++;
	}
	(*arr)[i] = NULL;
}

void	our_unset(char **cmds, char **copy_env)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (copy_env[i])
	{
		j = 1;
		while (cmds[j])
		{
			if (ft_strncmp(copy_env[i], cmds[j], ft_strlen(cmds[j])) == 0)
				if (copy_env[i][ft_strlen(cmds[j])] == '=')
					remove_elem(&copy_env, i);
			j++;
		}
		i++;
	}
	i = 0;
}
