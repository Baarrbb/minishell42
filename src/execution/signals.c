/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 19:51:03 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/25 12:20:02 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_ignore(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	printtestsignals(int status)
{
	if (WTERMSIG(status) == SIGINT)
		printf("C'était un SIGINT (Ctrl-C)\n");
	if (WIFEXITED(status))
	{
		printf("Le processus enfant s'est terminé normalement avec le code %d\n", WEXITSTATUS(status));
		//signal(SIGINT, handle_sigint);
	}
	else if (WIFSIGNALED(status) > 0)
	{
		printf("Le processus enfant a été terminé par le signal %d\n", WTERMSIG(status));
		//signal(SIGINT, handle_sigint);
	}
}
