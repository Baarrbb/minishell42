/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:07:33 by bsuc              #+#    #+#             */
/*   Updated: 2024/03/24 02:28:44 by marvin           ###   ########.fr       */
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

static char	*parse_home(char *pwd, char **env)
{
	char	*home;
	char	*parse_pwd;

	parse_pwd = 0;
	home = 0;
	home = get_ourenv_wo_equal("HOME", env);
	if (!home)
		return (ft_strdup(pwd));
	if (!ft_strncmp(pwd, home, ft_strlen(home)))
	{
		parse_pwd = strjoin(parse_pwd, "~");
		pwd += ft_strlen(home);
		free(home);
		if (!pwd)
			return (parse_pwd);
		parse_pwd = strjoin(parse_pwd, pwd);
		return (parse_pwd);
	}
	else
		return (free(home), ft_strdup(pwd));
}

static char	*display_prompt(char **env)
{
	char	*parse_pwd;
	char	*prompt;

	prompt = 0;
	parse_pwd = parse_home(get_ourenv_wo_alloc("PWD", env), env);
	prompt = strjoin(prompt, "minishell: ");
	prompt = strjoin(prompt, "\001" CYAN "\002");
	prompt = strjoin(prompt, parse_pwd);
	free(parse_pwd);
	prompt = strjoin(prompt, "\001" RESET "\002");
	prompt = strjoin(prompt, " $ ");
	return (prompt);
}

int	main(int ac, char **av, char **envp)
{
	char				*line;
	t_cmd				*pipe;
	char				**cpy_env;
	char				*prompt;
	struct sigaction	sa;
	struct sigaction	sb;
	int					sortie;

	(void)av;
	if (ac != 1)
		return (0);
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
		prompt = display_prompt(cpy_env);
		line = readline(prompt);
		rl_on_new_line();
		if (!line)
			return (free(prompt), our_exit(pipe, cpy_env, 0), 0);
		if (line[0] != ' ' && line[0] != 0)
			add_history(line);
		if (g_sigint_received == 2)
		{
			sortie = 130;
			g_sigint_received = 0;
		}
		check_line(line, &pipe, cpy_env);
		// print_linked_list(pipe);
		free(prompt);
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
