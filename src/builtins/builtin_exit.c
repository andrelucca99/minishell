/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:28:32 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/02 15:14:42 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	exit_code = 0;
	if (args[1])
	{
		if (args[2])
		{
			fprintf(stderr, "minishell: exit: too many arguments\n");
			shell->last_exit_status = 1;
			return (1);
		}
		exit_code = ft_atoi(args[1]);
	}
	shell->last_exit_status = exit_code;
	shell->running = 0;
	gc_clear();
	exit(exit_code);
}
