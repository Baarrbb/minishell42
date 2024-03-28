/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 16:27:29 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/28 12:25:22 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirs(t_cmd cmd, int *sortie)
{
	int	fd = -2;
	t_redir	*ptr;

	ptr = cmd.redir;
	if (ptr)
	{
		while (ptr)
		{
			if (ptr->in)
				fd = open(ptr->filename, O_RDONLY);
			else if (ptr->out)
				fd = open(ptr->filename, FILE_O_FLAGS_RDWR, FILE_PERM);
			else if (ptr->out_end)
				fd = open(ptr->filename, FILE_O_FLAGS, FILE_PERM);
			if (fd == -1)
			{
				*sortie = 1;
				ft_putstr_fd("minishell: ", 2);
				perror(ptr->filename);
				close(fd);
				return (0);
			}
			if (fd > 0)
				close(fd);
			ptr = ptr->next;
		}
	}
	return (1);
}

void	redirections_pipe_in(t_cmd *command, t_exec *data, int fd)
{
	if (command->next)
	{
		if (fd == -1)
		{
			if (dup2(data->pipefds[data->pipeindex + 1], 1) < 0)
			{
				perror("dup2 error to do");
				exit(EXIT_FAILURE);
			}
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
			open_file_in(cmd, data, &fd);
			cmd->redir = cmd->redir->next;
		}
		if (fd > 0)
			dup2(fd, 0);
		cmd->redir = oldredir;
		return (fd);
	}
	return (-1);
}

void	redirections_pipe_out(t_exec *data, int fd)
{
	if (data->index != 0)
	{
		if (fd == -1)
		{
			if (dup2(data->pipefds[data->pipeindex - 2], 0) < 0)
			{
				perror("dup2 error to do");
				exit(EXIT_FAILURE);
			}
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
			open_file_out(cmd->redir, &fd);
			cmd->redir = cmd->redir->next;
		}
		if (fd > 0)
			dup2(fd, 1);
		cmd->redir = oldredir;
		return (fd);
	}
	return (0);
}
