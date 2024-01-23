/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:07:33 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/23 21:59:26 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

void	handle_sigint(int sig) 
{
	g_sigint_received = 2;
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigout(int sig)
{
	g_sigint_received = 1;
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **envp)
{
	char				*line;
	t_cmd				*pipe;
	char				**cpy_env;
	struct sigaction	sa;
	struct sigaction	sb;
	int					sortie;

	(void)ac;
	(void)av;
	pipe = 0;
	sortie = 2;
	cpy_env = copy_env(envp);
	sb.sa_handler = handle_sigout;
	sb.sa_flags = 0;
	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigemptyset(&sb.sa_mask);
	while (1)
	{
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
		line = readline("Minishell $ ");
		rl_on_new_line();
		if (!line)
			return (our_exit(pipe, cpy_env), 0);
		if (line[0] != ' ' && line[0] != 0)
			add_history(line);
		if (g_sigint_received == 2)
		{
			sortie = 130;
			g_sigint_received = 0;
		}
		check_line(line, &pipe, cpy_env);
		if (pipe)
		{
			if (handle_quoting(pipe, cpy_env, sortie) == 0)
				return (printf("Error\n"), 0);
			check_commands(pipe);
			sortie = execute_test(pipe, &cpy_env);
			if (sortie == -1)
				return (printf("Error\n"), 0);
		}
		free_list(&pipe);
	}
	free_char_tab(cpy_env);
	return (0);
}
