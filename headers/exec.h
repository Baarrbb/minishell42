/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:14:45 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/26 16:46:33 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

# define FILE_O_FLAGS (O_WRONLY | O_APPEND | O_CREAT)
# define FILE_PERM (S_IRUSR | S_IWUSR)
# define FILE_O_FLAGS_RDWR (O_CREAT | O_RDWR | O_TRUNC)

void	free_struct_exec(t_exec *data);

/*test*/
int		execute_test(t_cmd *pipe, char ***envp);
int		check_commands(t_cmd *commands);
void	replace_variables(t_cmd *testons, char **envp);
int		handle_quoting(t_cmd *quoting, char **env, int sortie);
int		count_struct(t_cmd *list);
void	builtingo(t_cmd *cmd, char ***env, t_exec *data);
void	error_exec(t_cmd *comands);
void	error_managing(t_cmd *command, char **envp, t_cmd *start);
void	handle_sigint(int sig);
int		heredoc(t_cmd *test, t_exec *data);

/* handle variables*/
char	*get_ourenv(char *tofind, char **ourenv, char *sortie);
int		in_quotes(char test, int etat);
int		handle_var(t_cmd *cmd, char **our_envp, char *sortie);

/*redirections :*/
int		check_redirs(t_cmd cmd, int *sortie);
void	redirections_pipe_in(t_cmd *command, t_exec *data);
int		redirections_in(t_cmd *cmd, t_exec *data);
void	redirections_pipe_out(t_exec *data);
int		redirections_out(t_cmd *cmd);
/*pipes opening and closing*/
void	close_all_pipes(int numPipes, int *pipefds);
int		numbers_pipe(t_cmd *list);
void	creation_pipes(t_exec *yipi);
/*signals handling*/
void	sig_ignore(void);
void	sig_default(void);
void	printtestsignals(int status);
/*builtinpipe*/
void	builtinpipe(t_cmd *cmd, char ***env, t_exec *data);

#endif