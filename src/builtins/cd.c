/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 20:23:33 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/25 02:22:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	refresh_env_pwd(char ***env, char *pwd, char *oldpwd)
{
	char	**tmp;
	int		i;
	int		get_old;

	tmp = *env;
	i = -1;
	get_old = 0;
	while (tmp[++i])
	{
		if (!ft_strncmp(tmp[i], "OLDPWD=", ft_strlen("OLDPWD=")))
		{
			tmp[i] = update_env(tmp[i], "OLDPWD=", oldpwd);
			get_old = 1;
		}
		if (oldpwd && !ft_strncmp(tmp[i], "PWD=", ft_strlen("PWD=")))
			tmp[i] = update_env(tmp[i], "PWD=", pwd);
	}
	if (!get_old)
		put_old(env, oldpwd);
}

static int	ret_cd(int ret, char ***env, char *oldpwd, char *path)
{
	char	pwd[PATH_MAX];
	char	*err_pwd;

	if (!getcwd(pwd, PATH_MAX))
	{
		printf("%serror retrieving current directory: getcwd: cannot "
			"access parent directories: %s\n", ERROR_CD, strerror(errno));
		err_pwd = get_ourenv("OLDPWD=", *env, 0);
		if (err_pwd)
		{
			chdir(err_pwd);
			refresh_env_pwd(env, err_pwd, oldpwd);
			free(err_pwd);
		}
		return (1);
	}
	if (ret == 0)
		return (refresh_env_pwd(env, pwd, oldpwd), 0);
	else if (ret < 0)
		return (printf("%s%s: %s\n", ERROR_CD, path, strerror(errno)), 1);
	else
		return (1);
}

static int	move_cd(char *path, char ***env, char *oldpwd)
{
	int		ret;

	ret = 1;
	if (!path || !ft_strncmp(path, "--\0", 3) || !ft_strncmp(path, "~\0", 2)
		|| !ft_strncmp(path, "~/\0", 3))
	{
		if (ourenv_wo_alloc("HOME=", *env))
			ret = chdir(ourenv_wo_alloc("HOME", *env));
		else
			printf("%sHOME not set\n", ERROR_CD);
	}
	else if (!ft_strncmp(path, "-\0", 2) && ourenv_wo_alloc("OLDPWD", *env))
	{
		printf("%s\n", ourenv_wo_alloc("OLDPWD", *env));
		ret = chdir(ourenv_wo_alloc("OLDPWD", *env));
		return (ret_cd(ret, env, oldpwd, ourenv_wo_alloc("OLDPWD", *env)));
	}
	else if (!ft_strncmp(path, "-\0", 2) && !ourenv_wo_alloc("OLDPWD", *env))
		printf("%sOLDPWD not set\n", ERROR_CD);
	else if (path[0] == '-' && path[1] == '-' && path[3])
		printf("%s--: invalid option\n", ERROR_CD);
	else if (path[0] == '-' && path[1])
		printf("%s-%c: invalid option\n", ERROR_CD, path[1]);
	else
		ret = chdir(path);
	return (ret_cd(ret, env, oldpwd, path));
}

int	our_cd(t_cmd *cmd, char ***env)
{
	int		nb_args;
	char	*arg;
	char	*oldpwd;
	char	oldpwd_fail[PATH_MAX];
	int		ret;

	nb_args = 0;
	if (cmd->cmd && cmd->cmd[0])
	{
		while (cmd->cmd[nb_args])
			nb_args++;
	}
	if (nb_args > 2)
		return (printf("%stoo many arguments\n", ERROR_CD), 1);
	arg = cmd->cmd[1];
	oldpwd = get_ourenv("PWD=", *env, 0);
	if (!oldpwd && getcwd(oldpwd_fail, PATH_MAX))
		ret = move_cd(arg, env, oldpwd_fail);
	else
		ret = move_cd(arg, env, oldpwd);
	printf("retour %d\n", ret);
	if (oldpwd)
		free(oldpwd);
	return (ret);
}
