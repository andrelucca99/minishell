/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:28:32 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/03 15:38:13 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
		return (chdir(getenv("HOME")));
	else
		path = expand_variables(args[1], shell);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
