/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:37:17 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/12 19:03:35 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
