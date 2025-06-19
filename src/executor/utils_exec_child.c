/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec_child.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:23:27 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/19 14:24:33 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	redirect_to_output_file(t_command *cmd, t_shell *shell)
{
	int	out;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (cmd->append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	out = open(cmd->output_file, flags, 0644);
	if (out < 0)
	{
		perror("open output");
		gc_clear(&shell->gc);
		exit(1);
	}
	if (dup2(out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		gc_clear(&shell->gc);
		exit(1);
	}
	close(out);
}

static void	redirect_to_pipe(int fd[2])
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
}

void	setup_output_redir(t_command *cmd, int fd[2])
{
	t_shell	*shell;

	shell = get_shell();
	if (cmd->output_file)
		redirect_to_output_file(cmd, shell);
	else if (cmd->next)
		redirect_to_pipe(fd);
}
