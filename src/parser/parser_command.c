/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:16:30 by eschula           #+#    #+#             */
/*   Updated: 2025/06/11 19:30:56 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
