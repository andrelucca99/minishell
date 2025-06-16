/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:28:32 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/16 19:07:27 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	handle_exit_code(char **args, t_shell *shell)
{
	int	exit_code;

	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
			gc_clear(&shell->gc);
			exit(2);
		}
		exit_code = ft_atoi(args[1]) % 256;
		gc_clear(&shell->gc);
		exit(exit_code);
	}
	gc_clear(&shell->gc);
	exit(shell->last_exit_status);
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
	handle_exit_code(args, shell);
	return (0);
}
