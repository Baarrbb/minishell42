/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 20:23:33 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/23 21:56:24 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*update_env(char *line, char *new, char *path)
{
	free(line);
	line = 0;
	line = strjoin(line, new);
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
		if (!ft_strncmp(tmp[i], "PWD=", ft_strlen("PWD=")))
			tmp[i] = update_env(tmp[i], "PWD=", pwd);
	}
	if (!get_old)
	{
		line = 0;
		line = strjoin(line, "OLDPWD=");
		line = strjoin(line, oldpwd);
		put_var(env, line);
	}
}

static void	ret_cd(int ret, char ***env, char *oldpwd)
{
	char	pwd[PATH_MAX];

	getcwd(pwd, PATH_MAX);
	if (ret == 0)
		refresh_env_pwd(env, pwd, oldpwd);
	else if (ret < 0)
		perror(ERROR_CD);
	else
		return ;
}

static void	move_cd(char *path, char ***env, char *oldpwd)
{
	int		ret;

	ret = 1;
	if (!path || !ft_strncmp(path, "--\0", 3) || !ft_strncmp(path, "~\0", 2))
	{
		if (get_ourenv("HOME", *env, 0))
			ret = chdir(get_ourenv("HOME", *env, 0));
		else
			printf("%s HOME not set\n", ERROR_CD);
	}
	else if (!ft_strncmp(path, "-\0", 2) && get_ourenv("OLDPWD", *env, 0))
	{
		printf("%s\n", get_ourenv("OLDPWD", *env, 0));
		ret = chdir(get_ourenv("OLDPWD", *env, 0));
	}
	else if (!ft_strncmp(path, "-\0", 2) && !get_ourenv("OLDPWD", *env, 0))
		printf("%s OLDPWD not set\n", ERROR_CD);
	else if (path[0] == '-' && path[1] == '-' && path[3])
		printf("%s: --: invalid option\n", ERROR_CD);
	else if (path[0] == '-' && path[1])
		printf("%s: -%c: invalid option\n", ERROR_CD, path[1]);
	else
		ret = chdir(path);
	ret_cd(ret, env, oldpwd);
}

void	our_cd(t_cmd *cmd, char ***env)
{
	int		nb_args;
	char	*arg;
	char	oldpwd[PATH_MAX];

	nb_args = 0;
	if (cmd->cmd && cmd->cmd[0])
	{
		while (cmd->cmd[nb_args])
			nb_args++;
	}
	if (nb_args > 2)
	{
		printf("%s too many arguments\n", ERROR_CD);
		return ;
	}
	getcwd(oldpwd, PATH_MAX);
	arg = cmd->cmd[1];
	move_cd(arg, env, oldpwd);
}
