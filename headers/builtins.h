/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:14:03 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/23 19:39:28 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# define ERROR_CD "minishell: cd:"

/* ECHO_C */
void	our_echo(char **cmds_args);

/*  CD_C  */
void	our_cd(t_cmd *cmd, char **env);

/* PWD_C */
void	our_pwd(void);

/* EXPORT_C */
void	our_export(t_cmd *cmd, char ***env);

/* UNSET_C */
void	our_unset(char **cmds, char **copy_env);

/* ENV_C */
void	our_env(char **env);

/* EXIT_C */
void	our_exit(t_cmd *everything);

#endif