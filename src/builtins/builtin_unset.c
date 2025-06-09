/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:31:01 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/03 15:33:18 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_unset(char **args, t_shell *shell)
{
	int		i;
	char	*expanded;

	i = 1;
	while (args[i])
	{
		expanded = expand_variables(args[i], shell);
		unsetenv(expanded);
		i++;
	}
	return (0);
}
