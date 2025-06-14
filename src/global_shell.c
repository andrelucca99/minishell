/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:37:45 by eschula           #+#    #+#             */
/*   Updated: 2025/06/14 14:56:49 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_shell	*g_shell = NULL;

void	set_shell(t_shell *shell)
{
	g_shell = shell;
}

t_shell	*get_shell(void)
{
	return (g_shell);
}
