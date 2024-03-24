/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:37:27 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/24 02:11:18 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_ourenv_wo_alloc(char *tofind, char **env)
{
	int	i;

	if (env)
	{
		i = -1;
		while (env[++i])
		{
			if (!ft_strncmp(env[i], tofind, ft_strlen(tofind)))
			{
				return (env[i] + ft_strlen(tofind) + 1);
			}
		}
	}
	return (0);
}

char	*get_ourenv_wo_equal(char *tofind, char **env)
{
	int	i;

	if (env)
	{
		i = -1;
		while (env[++i])
		{
			if (!ft_strncmp(env[i], tofind, ft_strlen(tofind)))
			{
				return (ft_strdup(env[i] + ft_strlen(tofind) + 1));
			}
		}
	}
	return (0);
}

void	our_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		printf("getcwd() error");
}

void	our_exit(t_cmd *everything, char **env, t_exec *data)
{
	int	status;

	status = EXIT_SUCCESS;
	if (everything)
	{
		if (everything->cmd[1])
		{
			status = ft_atoi(everything->cmd[1]) % 256;
			if (everything->cmd[2])
			{
				status = 127;
				printf("exit\nbash: exit: too many arguments\n");
				return ;
			}
		}
		free_list(&everything);
	}
	free_char_tab(env);
	if (data)
	{
		close_all_pipes(data->numpipes, data->pipefds);
		free_struct_exec(data);
	}
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
	{
		close(fdinsave);
		close(fdoutsave);
		our_exit(cmd, *env, data);
	}
	dup2(fdoutsave, 1);
	dup2(fdinsave, 0);
}
