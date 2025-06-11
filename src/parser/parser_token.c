/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:26:01 by eschula           #+#    #+#             */
/*   Updated: 2025/06/11 18:48:51 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	extract_quoted_token(const char *line, int start, char quote, char **out)
{
	int	i;

	i = start + 1;
	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
		return (-1);
	if (out)
		*out = gc_strndup(&line[start + 1], i - start - 1);
	return (i + 1);
}

t_token	*new_token_with_expand(t_token_type type,
	char *value, int expand, int was_quoted)
{
	t_token	*token;

	token = gc_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->expand = expand;
	token->was_quoted = was_quoted;
	token->next = NULL;
	return (token);
}

int	handle_quoted_token(const char *line,
		int i, t_token **tokens, t_shell *shell)
{
	char	quote;
	char	*value;
	char	*expanded;
	int		next;

	quote = line[i];
	next = extract_quoted_token(line, i, quote, &value);
	if (next == -1)
	{
		fprintf(stderr, "minishell: erro de aspas não fechadas\n");
		return (i + 1);
	}
	if (quote == '"')
	{
		expanded = expand_variables(value, shell);
		add_token(tokens, new_token_with_expand(TOKEN_WORD, expanded, 0, 1));
	}
	else
		add_token(tokens, new_token_with_expand(TOKEN_WORD, value, 0, 1));
	return (next);
}
