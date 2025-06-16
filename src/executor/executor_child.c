/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:46:51 by eschula           #+#    #+#             */
/*   Updated: 2025/06/14 14:39:37 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_heredoc_input(t_command *cmd, t_shell *shell)
{
	int	in;

	in = handle_heredoc(cmd->heredoc_delim, cmd->heredoc_expand, shell);
	if (in < 0)
	{
		gc_clear(&shell->gc);
		exit(1);
	}
	dup2(in, STDIN_FILENO);
	close(in);
}

static void	handle_file_input(t_command *cmd)
{
	int		in;
	t_shell	*shell;

	shell = get_shell();
	in = open(cmd->input_file, O_RDONLY);
	if (in < 0)
	{
		perror("open input");
		gc_clear(&shell->gc);
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
	int		out;
	t_shell	*shell;

	shell = get_shell();
	if (cmd->output_file)
	{
		out = open(cmd->output_file,
				O_WRONLY | O_CREAT | (cmd->append_mode
					&& O_APPEND) | (!cmd->append_mode && O_TRUNC), 0644);
		if (out < 0)
		{
			perror("open output");
			gc_clear(&shell->gc);
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

void	setup_redirections(t_command *cmd, int fd_in, int fd[2], t_shell *shell)
{
	setup_input_redir(cmd, fd_in, shell);
	setup_output_redir(cmd, fd);
}
