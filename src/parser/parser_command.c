/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:16:30 by eschula           #+#    #+#             */
/*   Updated: 2025/06/11 18:16:31 by eschula          ###   ########.fr       */
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

static void	handle_token_word(
	t_token *tokens, t_shell *shell, char **argv, int *argc)
{
	char	*val;

	if (tokens->expand)
		val = expand_variables(tokens->value, shell);
	else
		val = tokens->value;
	argv[(*argc)++] = gc_strdup(val);
}

static void	handle_token_pipe(
	t_command **cmds, t_command **cur, char **argv, int *argc)
{
	finalize_command(cmds, cur, argv, *argc);
	*argc = 0;
}

static void	handle_token_else(
	t_token **tokens, t_command *cur)
{
	handle_redirection(tokens, cur);
}

void	parse_tokens_loop(
	t_token *tokens, t_shell *shell, t_command **cmds, t_command **cur)
{
	char	*argv[MAX_ARGS];
	int		argc;

	argc = 0;
	ft_bzero(argv, sizeof(argv));
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
			handle_token_word(tokens, shell, argv, &argc);
		else if (tokens->type == TOKEN_PIPE)
			handle_token_pipe(cmds, cur, argv, &argc);
		else
			handle_token_else(&tokens, *cur);
		tokens = tokens->next;
	}
	if (argc > 0)
		finalize_command(cmds, cur, argv, argc);
	else if (*cur && (*cur)->args == NULL)
		free(*cur);
	else
		add_command(cmds, *cur);
}

t_command	*parse_tokens(t_token *tokens, t_shell *shell)
{
	t_command	*cmds;
	t_command	*cur;

	cmds = NULL;
	cur = new_command();
	parse_tokens_loop(tokens, shell, &cmds, &cur);
	return (cmds);
}
