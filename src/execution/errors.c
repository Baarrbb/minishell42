/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:02:42 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/25 00:21:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	no_cmd_slash(t_cmd *command, char **envp)
{
	if (command->path_cmd == 0 && ft_strchr(command->cmd[0], '/') != 0)
	{
		errno = ENOENT;
		perror(command->cmd[0]);
		free_list(&command);
		free_char_tab(envp);
		exit(EXIT_FAILURE);
	}
}

static void	is_a_directory(t_cmd *command, char **envp)
{
	if (command->path_cmd)
	{
		if (command->path_cmd[ft_strlen(command->path_cmd) - 1] == '/')
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(command->path_cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			free_list(&command);
			free_char_tab(envp);
			exit(126);
		}
	}
}

static void	no_such_file_or_directory(t_cmd *command, char **envp)
{
	struct stat	sb;

	if (stat(command->cmd[0], &sb) == -1 && ft_strchr(command->cmd[0], '/') != 0)
	{
		printf("l39errors\n");
		perror(command->cmd[0]);
		free_list(&command);
		free_char_tab(envp);
		exit(127);
	}
}

static void	check_val_cmd(t_cmd *cmd, char **envp)
{
	if (!ft_strncmp(cmd->cmd[0], "..", 2)
		|| !cmd->cmd[0][0])
	{
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_list(&cmd);
		free_char_tab(envp);
		exit(127);
	}
	if (cmd->cmd[0][0] == '.' && !cmd->cmd[0][1])
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(": filename argument required\n", 2);
		free_list(&cmd);
		free_char_tab(envp);
		exit(2);
	}
}

static void	command_not_found(t_cmd *command, char **envp)
{
	int		i;
	char	*full_cmd;

	if (!command->path)
	{
		ft_putstr_fd(command->cmd[0], 2);
		// ft_putstr_fd(": command not found\n", 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free_list(&command);
		free_char_tab(envp);
		exit(127);
	}
	i = -1;
	while (command->path[++i])
	{
		full_cmd = strjoin(0, command->path[i]);
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
		free_char_tab(envp);
		exit(127);
	}
}

void	error_managing(t_cmd *command, char **envp)
{
	if (command->cmd == NULL)
	{
		free_list(&command);
		exit(1);
	}
	no_cmd_slash(command, envp);
	is_a_directory(command, envp);
	no_such_file_or_directory(command, envp);
	check_val_cmd(command, envp);
	command_not_found(command, envp);
	printf("there\n");
}
