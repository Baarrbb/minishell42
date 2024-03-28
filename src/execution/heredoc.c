/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:40:10 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/28 13:10:46 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_heredoc(t_exec *data, t_cmd **cmd, char **envp)
{
	free_struct_exec(data);
	free_list(cmd);
	free_char_tab(envp);
}

static void	reading_heredoc(t_heredoc hd)
{
	char	*line;

	close(hd.pipe[0]);
	while (1)
	{
		line = readline("testhd : ");
		if (line == NULL)
		{
			printf("ctrld ou bug\n");
			break ;
		}
		if (ft_strncmp(line, hd.delimiter, ft_strlen(hd.delimiter)) == 0)
		{
			if (line[ft_strlen(hd.delimiter)] == '\0')
			{
				free(line);
				break ;
			}
		}
		write(hd.pipe[1], line, ft_strlen(line));
		write(hd.pipe[1], "\n", 1);
	}
	close(hd.pipe[1]);
	//free_heredoc(data, cmd, envp);
	exit(0);
}

int	heredoc(t_cmd *test, t_exec *data)
{
	t_heredoc	heredoc;
	int		status;

	heredoc.delimiter = test->redir->filename;
	if (pipe(heredoc.pipe) < 0)
	{
		perror("fail pipe heredoc");
		return (-1);
	}
	heredoc.pid = fork();
	if (heredoc.pid == 0)
	{
		sig_default();
		close_all_pipes(data->numpipes, data->pipefds);
		reading_heredoc(heredoc);
	}
	if (heredoc.pid > 0)
	{
		close(heredoc.pipe[1]);
		waitpid(heredoc.pid, &status, 0);
		return (heredoc.pipe[0]);
	}
	return (-1);
}
