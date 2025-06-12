/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:28:32 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/12 15:21:09 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args, t_shell *shell)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	printf("exit\n");
	if (count > 2)
	{
		fprintf(stderr, "exit: too many arguments\n");
		shell->last_exit_status = 1;
		return (1);
	}
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
			exit(2);
		}
		exit(ft_atoi(args[1]) % 256);
	}
	exit(shell->last_exit_status);
}
