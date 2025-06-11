/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:12:01 by eschula           #+#    #+#             */
/*   Updated: 2025/06/11 19:22:25 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	finalize_command(
	t_command **cmds, t_command **cur, char *argv[], int argc)
{
	int	i;

	(*cur)->args = gc_malloc(sizeof(char *) * (argc + 1));
	if (!(*cur)->args)
	{
		i = 0;
		while (i < argc)
		{
			free(argv[i]);
			argv[i] = NULL;
			i++;
		}
		return ;
	}
	i = 0;
	while (i < argc)
	{
		(*cur)->args[i] = argv[i];
		argv[i] = NULL;
		i++;
	}
	(*cur)->args[argc] = NULL;
	add_command(cmds, *cur);
	*cur = new_command();
}

void	handle_token_word(
	t_token *tokens, t_shell *shell, char **argv, int *argc)
{
	char	*val;

	if (tokens->expand)
		val = expand_variables(tokens->value, shell);
	else
		val = tokens->value;
	argv[(*argc)++] = gc_strdup(val);
}

void	handle_token_pipe(
	t_command **cmds, t_command **cur, char **argv, int *argc)
{
	finalize_command(cmds, cur, argv, *argc);
	*argc = 0;
}

void	handle_token_else(
	t_token **tokens, t_command *cur)
{
	handle_redirection(tokens, cur);
}
