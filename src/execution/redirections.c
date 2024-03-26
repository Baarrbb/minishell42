/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 16:27:29 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/26 16:47:31 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirs(t_cmd cmd, int *sortie)
{
	int	fd = 0;
	
	if (cmd.redir)
	{
		while (cmd.redir)
		{
			if (cmd.redir->in)
				fd = open(cmd.redir->filename, O_RDONLY);
			else if (cmd.redir->out)
				fd = open(cmd.redir->filename, FILE_O_FLAGS_RDWR, FILE_PERM);
			else if (cmd.redir->out)
				fd = open(cmd.redir->filename, FILE_O_FLAGS, FILE_PERM);
			if (fd == -1)
			{
				*sortie = 1;
				ft_putstr_fd("minishell: ", 2);
				perror(cmd.redir->filename);
				close(fd);
				return (0);
			}
			close(fd);
			cmd.redir = cmd.redir->next;
		}
	}
	return (1);
}

void	redirections_pipe_in(t_cmd *command, t_exec *data)
{
	if (command->next)
	{
		if (dup2(data->pipefds[data->pipeindex + 1], 1) < 0)
		{
			perror("dup2 error to do");
			exit(EXIT_FAILURE);
		}
	}
}

int	redirections_in(t_cmd *cmd, t_exec *data)
{
	int		fd;
	t_redir	*oldredir;

	if (cmd->redir)
	{
		oldredir = cmd->redir;
		fd = -1;
		while (cmd->redir != NULL)
		{
			if (fd != -1 && fd > 2)
			{
				close(fd);
				fd = -1;
			}
			if (cmd->redir->in)
				fd = open(cmd->redir->filename, O_RDONLY);
			else if (cmd->redir->in_read)
				fd = heredoc(cmd, data);
			cmd->redir = cmd->redir->next;
		}
		if (fd > 2)
		{
			dup2(fd, 0);
			close(fd);
		}
		cmd->redir = oldredir;
	}
	return (0);
}

void	redirections_pipe_out(t_exec *data)
{
	if (data->index != 0)
	{
		if (dup2(data->pipefds[data->pipeindex - 2], 0) < 0)
		{
			perror("dup2 error to do");
			exit(EXIT_FAILURE);
		}
	}
}

int	redirections_out(t_cmd *cmd)
{
	t_redir	*oldredir;
	int		fd;

	if (cmd->redir)
	{
		fd = -1;
		oldredir = cmd->redir;
		while (cmd->redir != NULL)
		{
			if (fd > 2)
				close(fd);
			if (cmd->redir->out)
				fd = open(cmd->redir->filename, FILE_O_FLAGS_RDWR, FILE_PERM);
			else if (cmd->redir->out_end)
				fd = open(cmd->redir->filename, FILE_O_FLAGS, FILE_PERM);
			cmd->redir = cmd->redir->next;
		}
		dup2(fd, 1);
		if (fd > 2)
			close(fd);
		cmd->redir = oldredir;
		return (fd);
	}
	return (0);
}
