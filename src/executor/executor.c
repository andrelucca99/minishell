/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:37:17 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/09 13:20:07 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_heredoc_input(t_command *cmd, t_shell *shell)
{
	int	in;

	in = handle_heredoc(cmd->heredoc_delim, cmd->heredoc_expand, shell);
	if (in < 0)
	{
		gc_clear();
		exit(1);
	}
	dup2(in, STDIN_FILENO);
	close(in);
}

static void	handle_file_input(t_command *cmd)
{
	int	in;

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

static void	setup_input_redir(t_command *cmd, int fd_in, t_shell *shell)
{
	if (cmd->heredoc_delim)
		handle_heredoc_input(cmd, shell);
	else if (cmd->input_file)
		handle_file_input(cmd);
	else if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
}

static void	setup_output_redir(t_command *cmd, int fd[2])
{
	int	out;

	if (cmd->output_file)
	{
		out = open(cmd->output_file,
				O_WRONLY | O_CREAT | (cmd->append_mode
					&& O_APPEND) | (!cmd->append_mode && O_TRUNC), 0644);
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

static void	setup_redirections(t_command *cmd,
	int fd_in, int fd[2], t_shell *shell)
{
	setup_input_redir(cmd, fd_in, shell);
	setup_output_redir(cmd, fd);
}

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
		exit(127);
	}
	execve(path, cmd->args, environ);
	perror("execve");
	gc_clear();
	exit(1);
}

static void	handle_child_signal(int status)
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

static void	wait_for_children(t_shell *shell)
{
	int	status;

	while (wait(&status) > 0)
	{
		handle_child_signal(status);
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
	}
}

static int	execute_commands_loop(t_command *cmds, t_shell *shell)
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

void	execute_commands(t_command *cmds, t_shell *shell)
{
	if (should_execute_builtin_in_parent(cmds))
	{
		shell->last_exit_status = exec_builtin(cmds->args, shell);
		return ;
	}
	if (execute_commands_loop(cmds, shell) == -1)
		return ;
	wait_for_children(shell);
}
