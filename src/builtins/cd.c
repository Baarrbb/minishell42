/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 20:23:33 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/24 14:19:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*update_env(char *line, char *new, char *path)
{
	free(line);
	line = 0;
	line = strjoin(line, new);
	if (path)
		line = strjoin(line, path);
	return (line);
}

static void	refresh_env_pwd(char ***env, char *pwd, char *oldpwd)
{
	char	**tmp;
	int		i;
	int		get_old;
	char	*line;

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
	{
		line = strjoin(0, "OLDPWD=");
		line = strjoin(line, oldpwd);
		put_var(env, line, 0);
		free(line);
	}
}

static int	ret_cd(int ret, char ***env, char *oldpwd, char *path)
{
	char	pwd[PATH_MAX];
	char	*err_pwd;

	if (!getcwd(pwd, PATH_MAX))
	{
		printf("%serror retrieving current directory: getcwd: cannot "
			"access parent directories: %s\n", ERROR_CD, strerror(errno));
		err_pwd = get_ourenv_wo_equal("OLDPWD", *env);
		chdir(err_pwd);
		refresh_env_pwd(env, err_pwd, oldpwd);
		free(err_pwd);
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
	if (!path || !ft_strncmp(path, "--\0", 3) || !ft_strncmp(path, "~\0", 2))
	{
		if (get_ourenv_wo_alloc("HOME", *env))
			ret = chdir(get_ourenv_wo_alloc("HOME", *env));
		else
			printf("%sHOME not set\n", ERROR_CD);
	}
	else if (!ft_strncmp(path, "-\0", 2) && get_ourenv_wo_alloc("OLDPWD", *env))
	{
		printf("%s\n", get_ourenv_wo_alloc("OLDPWD", *env));
		ret = chdir(get_ourenv_wo_alloc("OLDPWD", *env));
		return (ret_cd(ret, env, oldpwd, get_ourenv_wo_alloc("OLDPWD", *env)));
	}
	else if (!ft_strncmp(path, "-\0", 2)
		&& !get_ourenv_wo_alloc("OLDPWD", *env))
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
	{
		printf("%stoo many arguments\n", ERROR_CD);
		return (1);
	}
	arg = cmd->cmd[1];
	oldpwd = get_ourenv_wo_equal("PWD", *env);
	// if (!oldpwd)
	// {
	// 	getcwd(oldpwd_fail, PATH_MAX);
	// 	ret = move_cd(arg, env, oldpwd_fail);
	// }
	// else
	ret = move_cd(arg, env, oldpwd);
	printf("retour %d\n", ret);
	free(oldpwd);
	return (ret);
}
