/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:50:29 by eschula           #+#    #+#             */
/*   Updated: 2025/06/17 18:02:36 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_child(t_command *cmd, int fd_in, int fd[2], t_shell *shell)
{
	char	*path;
	int		tmp;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
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
	execve(path, cmd->args, environ);
	perror("execve");
	gc_clear(&shell->gc);
	exit(1);
}

int	execute_commands_loop(t_command *cmds, t_shell *shell)
{
	int		fd[2];
	int		fd_in;
	pid_t	pid;

	fd_in = STDIN_FILENO;
	while (cmds)
	{
		if (cmds->next && pipe(fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
		pid = fork();
		if (pid == 0)
			execute_child(cmds, fd_in, fd, shell);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (cmds->next)
		{
			close(fd[1]);
			fd_in = fd[0];
		}
		cmds = cmds->next;
	}
	return (0);
}
