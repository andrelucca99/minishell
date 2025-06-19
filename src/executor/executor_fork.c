/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:50:29 by eschula           #+#    #+#             */
/*   Updated: 2025/06/19 19:07:38 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_child(t_command *cmd, int fd_in, int fd[2], t_shell *shell)
{
	char	*path;
	int		tmp;

	signal(SIGQUIT, SIG_DFL);
	if (cmd->heredoc_cnt)
		signal(SIGINT, SIG_IGN);
	setup_redirections(cmd, fd_in, fd, shell);
	if (is_builtin(cmd->args[0]))
	{
		tmp = exec_builtin(cmd->args, shell);
		exit(tmp);
	}
	path = find_executable(cmd->args[0]);
	if (!path)
	{
		fprintf(stderr, "command not found: %s\n", cmd->args[0]);
		gc_clear(&shell->gc);
		exit(127);
	}
	execve(path, cmd->args, shell->env);
	perror("execve");
	gc_clear(&shell->gc);
	exit(1);
}

static int	execute_single_command(t_command *cmd, int *fd_in, t_shell *shell)
{
	int	fd[2];

	if (prepare_pipe_and_signal(cmd, fd) == -1)
		return (-1);
	if (fork_and_execute(cmd, fd_in, fd, shell) == -1)
		return (-1);
	return (0);
}

int	execute_commands_loop(t_command *cmds, t_shell *shell)
{
	int	fd_in;

	fd_in = STDIN_FILENO;
	while (cmds)
	{
		if (execute_single_command(cmds, &fd_in, shell) == -1)
			return (-1);
		cmds = cmds->next;
	}
	return (0);
}
