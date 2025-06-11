/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:16:54 by eschula           #+#    #+#             */
/*   Updated: 2025/06/11 18:16:55 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_heredoc_redir(t_token **tokens, t_command *cur)
{
	*tokens = (*tokens)->next;
	cur->heredoc_delim = gc_strdup((*tokens)->value);
	if ((*tokens)->was_quoted)
		cur->heredoc_expand = 0;
	else
		cur->heredoc_expand = 1;
}

void	handle_redirection(t_token **tokens, t_command *cur)
{
	t_token	*t;

	t = *tokens;
	if (t->type == TOKEN_REDIR_IN && t->next)
	{
		*tokens = t->next;
		cur->input_file = gc_strdup((*tokens)->value);
	}
	else if (t->type == TOKEN_REDIR_OUT && t->next)
	{
		*tokens = t->next;
		cur->output_file = gc_strdup((*tokens)->value);
	}
	else if (t->type == TOKEN_REDIR_APPEND && t->next)
	{
		*tokens = t->next;
		cur->output_file = gc_strdup((*tokens)->value);
		cur->append_mode = 1;
	}
	else if (t->type == TOKEN_HEREDOC && t->next)
		handle_heredoc_redir(tokens, cur);
}
