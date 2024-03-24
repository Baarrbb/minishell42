/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:38:44 by marvin            #+#    #+#             */
/*   Updated: 2024/03/24 17:38:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*update_env(char *line, char *new, char *path)
{
	free(line);
	line = 0;
	line = strjoin(line, new);
	if (path)
		line = strjoin(line, path);
	return (line);
}

void	put_old(char ***env, char *oldpwd)
{
	char	*line;

	line = strjoin(0, "OLDPWD=");
	if (oldpwd)
		line = strjoin(line, oldpwd);
	free(oldpwd);
	put_var(env, line, 0);
	free(line);
}
