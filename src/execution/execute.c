/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:53:56 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/28 12:47:29 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_sigint_received;

void	fill_builtins(t_exec *data, t_cmd *command)
{
	int	i;

	i = 0;
	data->builtin = malloc((data->numpipes) * sizeof(int));
	while (command)
	{
		if (command->builtin == 1)
			data->builtin[i] = 1;
		else
			data->builtin[i] = 0;
		command = command->next;
		i++;
	}
}

static t_exec	*fill_struct_exec(t_cmd *command)
{
	t_exec	*data;

	data = malloc(1 * sizeof(t_exec));
	if (data == NULL)
		return (0);
	data->index = 0;
	data->numpipes = numbers_pipe(command);
	data->pid = malloc((data->numpipes + 1) * sizeof(pid_t));
	if (data->pid == NULL)
	{
		free(data);
		return (0);
	}
	data->pid[data->numpipes] = '\0';
	fill_builtins(data, command);
	data->pipefds = malloc(((data->numpipes * 2)) * sizeof(int));
	if (data->pipefds == NULL)
	{
		free(data->pid);
		free(data);
		return (0);
	}
	data->pipeindex = 0;
	creation_pipes(data);
	return (data);
}

void	free_struct_exec(t_exec *data)
{
	free(data->pipefds);
	free(data->pid);
	if (data->builtin)
	{
		free(data->builtin);
	}
	free(data);
}

void	exec_utils(t_cmd *command, t_exec *data, char ***envp, t_cmd *start)
{
	int	i;
	int	fd;
	int	fd_out;

	i = 0;
	sig_default();
	command->fdinopen = redirections_in(command, data);
	redirections_pipe_in(command, data, command->fdinopen);
	command->fdoutopen = redirections_out(command);
	redirections_pipe_out(data, command->fdoutopen);
	close_all_pipes(data->numpipes, data->pipefds);
	free_struct_exec(data);
	data = 0;
	if (command->builtin == 1)
	{
		builtinpipe(command, envp, data);
		i = command->exit_val;
		free_list(&start);
		free_char_tab(*envp);
		exit(i);
	}
}

static void	exec(t_cmd *command, t_exec *data, char **envp, t_cmd *start)
{
	data->pid[data->index] = fork();
	if (data->pid[data->index] == 0)
	{
		exec_utils(command, data, &envp, start);
		error_managing(command, envp, start);
		if (execve(command->path_cmd, command->cmd, envp) < 0)
		{
			perror(command->path_cmd);
			free_list(&command);
			free_char_tab(envp);
			exit(126);
		}
	}
	else if (data->pid[data->index] < 0)
	{
		perror("fork error to do");
		free_list(&command);
		free_char_tab(envp);
		exit(EXIT_FAILURE);
	}
}

static int	handle_waitpid(t_cmd *pipe, t_exec *data)
{
	int	status;
	int	wait_result;

	status = 0;
	wait_result = 0;
	data->index = 0;
	while (data->index < data->numpipes)
	{
		if (data->builtin[data->index] == 0 || data->numpipes > 1)
			wait_result = waitpid(data->pid[data->index], &status, 0);
		if (wait_result == -1)
		{
			perror("waitpid error");
			break ;
		}
		if (WIFSIGNALED(status))
			if (WTERMSIG(status) == SIGINT)
				return (130);
		data->index++;
	}
	if (data->builtin[data->index - 1] == 1 && data->numpipes <= 1)
		return (pipe->exit_val);
	return (WEXITSTATUS(status));
}

int	execute_test(t_cmd *pipe, char ***envp)
{
	t_exec	*data;
	t_cmd	*command;
	t_cmd	*start;
	int		status;

	command = (t_cmd *)pipe;
	status = 0;
	start = command;
	data = fill_struct_exec(command);
	if (data == NULL)
		return (-1);
	while (command)
	{
		sig_ignore();
		if (command->builtin && data->numpipes <= 1)
			builtingo(command, envp, data);
		else
			exec(command, data, *envp, start);
		command = command->next;
		data->pipeindex += 2;
		data->index++;
	}
	close_all_pipes(data->numpipes, data->pipefds);
	status = handle_waitpid(pipe, data);
	printtestsignals(status);
	free_struct_exec(data);
	return (status);
}
