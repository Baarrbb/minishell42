/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:37:27 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/04 14:34:00 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	our_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		printf("getcwd() error");
}

void	our_exit(t_cmd *everything, char **env)
{
	int	status;

	status = EXIT_SUCCESS;
	if (everything->cmd[1])
		status = ft_atoi(everything->cmd[1]) % 256;
	if (everything->cmd[2])
		printf("exit\nbash: exit: too many arguments");
	else
		free_list(&everything);
		free_char_tab(env);
		exit(status);
}

void	builtingo(t_cmd *cmd, char ***env, t_exec *data)
{
	int	fdinsave;
	int	fdoutsave;

	fdinsave = dup(0);
	fdoutsave = dup(1);
	redirections_pipe_in(cmd, data);
	redirections_in(cmd, data);
	redirections_out(cmd);
	redirections_pipe_out(data);
	if (!ft_strncmp(cmd->cmd[0], "echo", ft_strlen("echo")))
		our_echo(cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "cd", ft_strlen("cd")))
		our_cd(cmd, env);
	else if (!ft_strncmp(cmd->cmd[0], "pwd", ft_strlen("pwd")))
		our_pwd();
	else if (!ft_strncmp(cmd->cmd[0], "export", ft_strlen("export")))
		our_export(cmd, env);
	else if (!ft_strncmp(cmd->cmd[0], "unset", ft_strlen("unset")))
		our_unset(cmd->cmd, *env);
	else if (!ft_strncmp(cmd->cmd[0], "env", ft_strlen("env")))
		our_env(*env);
	else if (!ft_strncmp(cmd->cmd[0], "exit", ft_strlen("exit")))
		our_exit(cmd, *env);
	dup2(fdoutsave, 1);
	dup2(fdinsave, 0);
	//close(fdoutsave);
	//close(fdinsave);
}
