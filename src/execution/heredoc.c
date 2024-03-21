/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:40:10 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/21 10:55:55 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	reading_heredoc(int *pipeheredoc, char *delimiter)
{
	char	*line;

	close(pipeheredoc[0]);
	while (1)
	{
		line = readline("testheredoc : ");
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			if (line[ft_strlen(delimiter)] == '\0')
			{
				free(line);
				break ;
			}
		}
		write(pipeheredoc[1], line, ft_strlen(line));
		write(pipeheredoc[1], "\n", 1);
	}
	close(pipeheredoc[1]);
	exit(0);
}

int	heredoc(t_cmd *test, t_exec *data)
{
	int		pipeheredoc[2];
	char	*delimiter;
	pid_t	pid;
	int		status;

	delimiter = test->redir->filename;
	signal(SIGQUIT, SIG_IGN);
	if (pipe(pipeheredoc) < 0)
	{
		perror("fail pipe");
		return (0);
	}
	pid = fork();
	if (pid == 0)
	{
		close_all_pipes(data->numpipes, data->pipefds);
		reading_heredoc(pipeheredoc, delimiter);
	}
	if (pid > 0)
	{
		close(pipeheredoc[1]);
		waitpid(pid, &status, 0);
		return (pipeheredoc[0]);
	}
	return (0);
}
