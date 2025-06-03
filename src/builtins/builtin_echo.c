/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:28:32 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/03 15:18:59 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_echo(char **args, t_shell *shell)
{
	int	i;

	i = 1;
	while (args[i])
	{
		char *expanded = expand_variables(args[i], shell);
		printf("%s", expanded);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}
