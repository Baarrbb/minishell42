/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:20:22 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/27 13:29:01 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_file_out(t_redir *redir, int *fd)
{
	if (redir->out)
	{
		if (*fd > 2)
			close(*fd);
		*fd = open(redir->filename, FILE_O_FLAGS_RDWR, FILE_PERM);
	}
	else if (redir->out_end)
	{
		if (*fd > 2)
			close(*fd);
		*fd = open(redir->filename, FILE_O_FLAGS, FILE_PERM);
	}
}

void	open_file_in(t_cmd *cmd, t_exec *data, int *fd)
{
	if (cmd->redir->in)
	{
		if (*fd > 2)
			close(*fd);
		*fd = open(cmd->redir->filename, O_RDONLY);
	}
	else if (cmd->redir->in_read)
	{
		if (*fd > 2)
			close(*fd);
		*fd = heredoc(cmd, data);
	}
}