/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:37:27 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/26 19:16:50 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ourenv_wo_alloc(char *tofind, char **env)
{
	int	i;

	if (env)
	{
		i = -1;
		while (env[++i])
		{
			if (ft_strchr(env[i], '='))
			{
				if (!ft_strncmp(env[i], tofind, ft_strlen(tofind)))
				{
					return (env[i] + ft_strlen(tofind) + 1);
				}
			}
			
		}
	}
	return (0);
}

// char	*get_ourenv_wo_equal(char *tofind, char **env)
// {
// 	int		i;
// 	char	*ret;
// 	char	c;
// 	if (env)
// 	{
// 		i = -1;
// 		while (env[++i])
// 		{
// 			if (!ft_strncmp(env[i], tofind, ft_strlen(tofind)))
// 			{
// 				if (env[i] + ft_strlen(tofind) + 1)
// 				{
// 					c = *(env[i] + ft_strlen(tofind) + 1);
// 					if (c != '\0')
// 					{
// 						ret = ft_strdup(env[i] + ft_strlen(tofind) + 1);
// 						if (ret && ret[0])
// 							return (ft_strdup(ret));
// 					}
// 				}
// 				else
// 					return (NULL);
// 			}
// 		}
// 	}
// 	return (0);
// }

int	our_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		printf("getcwd() error");
	return (0);
}

static int	check_val_exit(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (i == 0 && (cmd[0] == '-' || cmd[0] == '+'))
			continue ;
		if (!ft_isdigit(cmd[i]))
		{
			printf("minishell: exit: %s: numeric argument required\n", cmd);
			return (1);
		}
	}
	return (0);
}

int	our_exit(t_cmd *everything, char **env, t_exec *data)
{
	int	status;

	status = EXIT_SUCCESS;
	if (everything)
	{
		printf("exit\n");
		if (everything->cmd[1])
		{
			if (check_val_exit(everything->cmd[1]))
				status = 2;
			else
			{
				status = ft_atoi(everything->cmd[1]) % 256;
				if (everything->cmd[2])
				{
					printf("minishell: exit: too many arguments\n");
					return (everything->last_exit_status);
				}
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
void	command_exec(t_cmd *cmd, char ***env, t_exec *data, int *fdsave)
{
	if (!ft_strncmp(cmd->cmd[0], "echo", ft_strlen("echo")))
		cmd->exit_val = our_echo(cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "cd", ft_strlen("cd")))
		cmd->exit_val = our_cd(cmd, env);
	else if (!ft_strncmp(cmd->cmd[0], "pwd", ft_strlen("pwd")))
		cmd->exit_val = our_pwd();
	else if (!ft_strncmp(cmd->cmd[0], "export", ft_strlen("export")))
		cmd->exit_val = our_export(cmd, env);
	else if (!ft_strncmp(cmd->cmd[0], "unset", ft_strlen("unset")))
		cmd->exit_val = our_unset(cmd->cmd, *env);
	else if (!ft_strncmp(cmd->cmd[0], "env", ft_strlen("env")))
		cmd->exit_val = our_env(*env);
	else if (!ft_strncmp(cmd->cmd[0], "exit", ft_strlen("exit")))
	{
		close(fdsave[0]);
		close(fdsave[1]);
		cmd->exit_val = our_exit(cmd, *env, data);
	}
}

void	builtingo(t_cmd *cmd, char ***env, t_exec *data)
{
	int	fdsave[2];

	fdsave[0] = dup(0);
	fdsave[1] = dup(1);
	redirections_pipe_in(cmd, data);
	redirections_in(cmd, data);
	redirections_out(cmd);
	redirections_pipe_out(data);
	command_exec(cmd, env, data, fdsave);
	dup2(fdsave[1], 1);
	dup2(fdsave[0], 0);
	close(fdsave[1]);
	close(fdsave[0]);
}
