/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:05:23 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/24 23:38:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	good_flag(char *arguments)
{
	int	i;

	i = 1;
	if (!arguments)
		return (1);
	if (arguments[0] != '-')
		return (1);
	while (arguments[i])
	{
		if (arguments[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	our_echo(char **cmds_args)
{
	int	next_line;
	int	i;

	i = 1;
	next_line = 1;
	if (cmds_args[1] == NULL)
	{
		printf("\n");
		return ;
	}
	while (good_flag(cmds_args[i]) == 0)
	{
		next_line = 0;
		i++;
	}
	while (cmds_args[i])
	{
		printf("%s", cmds_args[i]);
		if (cmds_args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (next_line == 1)
		printf("\n");
}
