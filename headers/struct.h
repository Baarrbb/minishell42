/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:03:51 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/28 13:09:45 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct s_redir
{
	int				out;
	int				out_end;
	int				in;
	int				in_read;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**path;
	char			**cmd;
	int				builtin;
	char			*path_cmd;
	int				exit_val;
	int				last_exit_status;
	int				var_env;
	int				fdinopen;
	int				fdoutopen;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_exec
{
	t_cmd	*command;
	int		numpipes;
	int		index;
	pid_t	*pid;
	int		*builtin;
	int		*pipefds;
	int		fd;
	int		pipeindex;
}	t_exec;

typedef struct s_heredoc
{
	char	*delimiter;
	int		pipe[2];
	pid_t	pid;
}	t_heredoc;

#endif