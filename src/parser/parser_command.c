/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:16:30 by eschula           #+#    #+#             */
/*   Updated: 2025/06/19 19:16:27 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	heredoc_counter(t_token *tokens)
{
	int		cnt;
	t_token	*token_tmp;

	token_tmp = tokens;
	cnt = 0;
	while (token_tmp)
	{
		if (token_tmp->type == TOKEN_HEREDOC)
			cnt++;
		token_tmp = token_tmp->next;
	}
	return (cnt);
}

void	parse_tokens_loop(
	t_token *tokens, t_shell *shell, t_command **cmds, t_command **cur)
{
	char		*argv[MAX_ARGS];
	t_token		*token_head;
	int			argc;

	argc = 0;
	token_head = tokens;
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
	(*cmds)->heredoc_cnt = heredoc_counter(token_head);
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
