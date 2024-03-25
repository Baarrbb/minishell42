/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:02:42 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/25 17:19:28 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	no_cmd_slash(t_cmd *command, char **envp, t_cmd *start)
{
	if (command->path_cmd == 0 && ft_strchr(command->cmd[0], '/') != 0)
	{
		errno = ENOENT;
		perror(command->cmd[0]);
		free_list(&start);
		free_char_tab(envp);
		exit(EXIT_FAILURE);
	}
}		

static void	is_a_directory(t_cmd *command, char **envp, t_cmd *start)
{
	if (command->path_cmd)
	{
		if (command->path_cmd[ft_strlen(command->path_cmd) - 1] == '/')
		{
			ft_putstr_fd("bash: ", 2);
			ft_putstr_fd(command->path_cmd, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			free_list(&start);
			free_char_tab(envp);
			exit(126);
		}
	}
}

static void	no_such_file_or_directory(t_cmd *cmd, char **envp, t_cmd *start)
{
	struct stat	sb;

	if (stat(cmd->cmd[0], &sb) == -1 && ft_strchr(cmd->cmd[0], '/') != 0)
	{
		printf("l39errors\n");
		perror(cmd->cmd[0]);
		free_list(&start);
		free_char_tab(envp);
		exit(127);
	}
}

static void	check_val_cmd(t_cmd *cmd, char **envp, t_cmd *start)
{
	if (!ft_strncmp(cmd->cmd[0], "..", 2)
		|| !cmd->cmd[0][0])
	{
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_list(&start);
		free_char_tab(envp);
		exit(127);
	}
	if (cmd->cmd[0][0] == '.' && !cmd->cmd[0][1])
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->cmd[0], 2);
		ft_putstr_fd(": filename argument required\n", 2);
		free_list(&start);
		free_char_tab(envp);
		exit(2);
	}
}

static void	command_not_found(t_cmd *command, char **envp, t_cmd *start)
{
	int		i;
	char	*full_cmd;

	if (!command->path)
	{
		ft_putstr_fd(command->cmd[0], 2);
		// ft_putstr_fd(": command not found\n", 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free_list(&start);
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
		free_list(&start);
		free_char_tab(envp);
		exit(127);
	}
}

void	error_managing(t_cmd *command, char **envp, t_cmd *start)
{
	if (command->cmd == NULL)
	{
		free_list(&start);
		exit(1);
	}
	no_cmd_slash(command, envp, start);
	is_a_directory(command, envp, start);
	no_such_file_or_directory(command, envp, start);
	check_val_cmd(command, envp, start);
	command_not_found(command, envp, start);
}
