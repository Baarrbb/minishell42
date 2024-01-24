/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 19:03:51 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/24 18:07:18 by bsuc             ###   ########.fr       */
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
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_exec
{
	t_cmd	*command;
	int		numpipes;
	int		index;
	pid_t	*pid;
	int		*pipefds;
	int		fd;
	int		pipeindex;
}	t_exec;

#endif