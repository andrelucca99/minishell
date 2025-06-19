/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec_fork.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 19:03:31 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/19 19:04:40 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	std_cmd_c(int sig)
{
	(void)sig;
	return ;
}

int	prepare_pipe_and_signal(t_command *cmd, int fd[2])
{
	if (cmd->next && pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (cmd->heredoc_cnt)
		signal(SIGINT, SIG_IGN);
	else
		signal(SIGINT, std_cmd_c);
	return (0);
}

int	fork_and_execute(
	t_command *cmd, int *fd_in, int fd[2], t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		execute_child(cmd, *fd_in, fd, shell);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	if (cmd->next)
	{
		close(fd[1]);
		*fd_in = fd[0];
	}
	else
		*fd_in = STDIN_FILENO;
	return (0);
}
