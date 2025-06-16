/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:10:46 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/14 14:43:12 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*new_token(t_token_type type, const char *value)
{
	t_token	*token;
	t_shell	*shell;

	shell = get_shell();
	token = gc_malloc(&shell->gc, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = gc_strdup(value);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

t_token_type	get_operator_type(const char *s, int *len)
{
	if (s[0] == '>' && s[1] == '>')
		return (*len = 2, TOKEN_REDIR_APPEND);
	if (s[0] == '<' && s[1] == '<')
		return (*len = 2, TOKEN_HEREDOC);
	if (s[0] == '>')
		return (*len = 1, TOKEN_REDIR_OUT);
	if (s[0] == '<')
		return (*len = 1, TOKEN_REDIR_IN);
	if (s[0] == '|')
		return (*len = 1, TOKEN_PIPE);
	return (TOKEN_WORD);
}
