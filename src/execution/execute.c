/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:53:56 by ytouihar          #+#    #+#             */
/*   Updated: 2024/03/24 17:44:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	exec(t_cmd *command, t_exec *data, char **envp)
{
	data->pid[data->index] = fork();
	if (data->pid[data->index] == 0)
	{
		sig_default();
		redirections_pipe_in(command, data);
		redirections_in(command, data);
		redirections_out(command);
		printf("test\n");
		redirections_pipe_out(data);
		close_all_pipes(data->numpipes, data->pipefds);
		free_struct_exec(data);
		//close(0);
		error_managing(command, envp);
		if (execve(command->path_cmd, command->cmd, envp) < 0)
		{
			perror(command->path_cmd);
			free_list(&command);
			free_char_tab(envp);
			exit(127);
		}
	}
	else if (data->pid[data->index] < 0)
	{
		perror("dup2 error to do");
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
		if (data->builtin[data->index] == 0)
			wait_result = waitpid(data->pid[data->index], &status, 0);
		if (wait_result == -1)
		{
			perror("waitpid error");
			break ;
		}
		pipe->exit_val = wait_result;
		data->index++;
	}
	printf("WHAT\n");
	return (status);
}

int	execute_test(t_cmd *pipe, char ***envp)
{
	t_exec	*data;
	t_cmd	*command;
	int		status;

	command = (t_cmd *)pipe;
	status = 0;
	data = fill_struct_exec(command);
	if (data == NULL)
		return (-1);
	while (command)
	{
		sig_ignore();
		if (command->builtin)
			builtingo(command, envp, data);
		else
			exec(command, data, *envp);
		command = command->next;
		data->pipeindex += 2;
		data->index++;
	}
	close_all_pipes(data->numpipes, data->pipefds);
	status = handle_waitpid(pipe, data);
	printtestsignals(status);
	free_struct_exec(data);
	return (WEXITSTATUS(status));
}
