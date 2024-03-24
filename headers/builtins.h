/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:14:03 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/24 17:39:10 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# define ERROR_CD "minishell: cd: "
# define ERROR_EXPORT "minishell: export: "

/* BUILTINS.C */
char	*get_ourenv_wo_equal(char *tofind, char **env);
char	*ourenv_wo_alloc(char *tofind, char **env);

/* ECHO_C */
void	our_echo(char **cmds_args);

/*  CD_C  */
int	our_cd(t_cmd *cmd, char ***env);

/* CD_UTILS.C */
char	*update_env(char *line, char *new, char *path);
void	put_old(char ***env, char *oldpwd);

/* PWD_C */
void	our_pwd(void);

/* EXPORT_C */
void	put_var(char ***env, char *var, int add);
int		our_export(t_cmd *cmd, char ***env);

/* EXPORT_PRINT.C */
void	print_export_alpha(char **env, int size);

/* EXPORT_UTILS.C */
char	*get_var_wo_add(char *var);
char	*get_name_var(char *var, int add);
char	*get_value(char *var);
int		cmp_name(char *var1, char *var2);
int		get_size(char **env);

/* UNSET_C */
void	our_unset(char **cmds, char **copy_env);

/* ENV_C */
void	our_env(char **env);

/* EXIT_C */
void	our_exit(t_cmd *everything, char **env, t_exec *data);

#endif