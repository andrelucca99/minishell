/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_wait.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:52:10 by eschula           #+#    #+#             */
/*   Updated: 2025/06/12 19:00:37 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_child_signal(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 20);
		else if (sig == SIGINT)
			write(STDERR_FILENO, "\n", 1);
	}
}

void	wait_for_children(t_shell *shell)
{
	int	status;

	while (wait(&status) > 0)
	{
		handle_child_signal(status);
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
	}
}
