/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ersees <ersees@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:02:42 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/22 17:16:32 by ersees           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	no_cmd_slash(t_cmd *command)
{
	if (command->path_cmd == 0 && ft_strchr(command->cmd[0], '/') != 0)
	{
		errno = ENOENT;
		perror(command->cmd[0]);
		free_list(&command);
		exit(EXIT_FAILURE);
	}
}

static void	is_a_directory(t_cmd *command)
{
	if (command->path_cmd)
	{
		if (command->path_cmd[ft_strlen(command->path_cmd) - 1] == '/')
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(command->path_cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			free_list(&command);
			exit(126);
		}
	}
}

static void	no_such_file_or_directory(t_cmd *command)
{
	struct stat	sb;

	if (stat(command->cmd[0], &sb) == -1 && ft_strchr(command->cmd[0], '/') != 0)
	{
		printf("l39errors\n");
		perror(command->cmd[0]);
		free_list(&command);
		exit(127);
	}
}

static void	command_not_found(t_cmd *command)
{
	int		i;
	char	*full_cmd;

	full_cmd = 0;
	i = -1;
	while (command->path[++i])
	{
		full_cmd = strjoin(full_cmd, command->path[i]);
		full_cmd = strjoin(full_cmd, "/");
		full_cmd = strjoin(full_cmd, command->cmd[0]);
		if (access(full_cmd, X_OK) == 0)
		{
			command->path_cmd = full_cmd;
			break ;
		}
		free(full_cmd);
		full_cmd = 0;
	}
	if (!command->path_cmd)
	{
		ft_putstr_fd(command->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_list(&command);
		exit(127);
	}
}

void	error_managing(t_cmd *command)
{
	if (command->cmd == NULL)
	{
		free_list(&command);
		exit(1);
	}
	no_cmd_slash(command);
	is_a_directory(command);
	no_such_file_or_directory(command);
	command_not_found(command);
	printf("there\n");
}
