/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:37:17 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/27 16:12:15 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	setup_redirections(t_command *cmd, int fd_in, int fd[2])
{
	int	in;
	int	out;

	if (cmd->input_file)
	{
		in = open(cmd->input_file, O_RDONLY);
		if (in < 0)
		{
			perror("open input");
			gc_clear();
			exit(1);
		}
		dup2(in, STDIN_FILENO);
		close(in);
	}
	else if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}

	if (cmd->output_file)
	{
		out = open(cmd->output_file,
			O_WRONLY | O_CREAT | (cmd->append_mode ? O_APPEND : O_TRUNC), 0644); // trocar ternário
		if (out < 0)
		{
			perror("open output");
			gc_clear();
			exit(1);
		}
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	else if (cmd->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

static void	execute_child(t_command *cmd, int fd_in, int fd[2])
{
	char	*path;
	int		tmp;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);

	setup_redirections(cmd, fd_in, fd);

	if (is_builtin(cmd->args[0]))
		{
			tmp = exec_builtin(cmd->args);
			gc_clear();
			exit(tmp);
		}

	path = find_executable(cmd->args[0]);
	if (!path)
	{
		fprintf(stderr, "command not found: %s\n", cmd->args[0]);
		gc_clear();
		exit(127);
	}
	execve(path, cmd->args, environ);
	perror("execve");
	gc_clear();
	exit(1);
}

static void	wait_for_children(void)
{
	int	status;
	int	sig;

	while (wait(&status) > 0)
	{
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(STDERR_FILENO, "Quit (core dumped)\n", 20);
			else if (sig == SIGINT)
				write(STDERR_FILENO, "\n", 1);
		}
	}
}

void	execute_commands(t_command *cmds)
{
	int		fd[2];
	int		fd_in;
	pid_t	pid;

	fd_in = STDIN_FILENO;
	if (should_execute_builtin_in_parent(cmds))
	{
		exec_builtin(cmds->args);
		return ;
	}

	while (cmds)
	{
		if (cmds->next && pipe(fd) == -1)
		{
			perror("pipe");
			return ;
		}
		pid = fork();
		if (pid == 0)
			execute_child(cmds, fd_in, fd);
		if (fd_in != STDIN_FILENO)
			close(fd_in);
		if (cmds->next)
		{
			close(fd[1]);
			fd_in = fd[0];
		}
		cmds = cmds->next;
	}
	wait_for_children();
}
