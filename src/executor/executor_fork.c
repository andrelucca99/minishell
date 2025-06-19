/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:50:29 by eschula           #+#    #+#             */
/*   Updated: 2025/06/19 18:58:06 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	std_cmd_c(int sig)
{
	(void)sig;
	return ;
}

static void	execute_child(t_command *cmd, int fd_in, int fd[2], t_shell *shell)
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
	int		fd[2];
	pid_t	pid;

	if (cmd->next && pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (cmd->heredoc_cnt)
		signal(SIGINT, SIG_IGN);
	else
		signal(SIGINT, std_cmd_c);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
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
