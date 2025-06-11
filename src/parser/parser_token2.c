/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:30:16 by eschula           #+#    #+#             */
/*   Updated: 2025/06/11 18:35:02 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_operator_token(const char *line, int i, t_token **tokens)
{
	t_token_type	type;
	char			*sym;
	int				op_len;

	type = get_operator_type(&line[i], &op_len);
	if (type != TOKEN_WORD)
	{
		sym = gc_strndup(&line[i], op_len);
		add_token(tokens, new_token_with_expand(type, sym, 0, 0));
		return (i + op_len);
	}
	return (-1);
}

int	handle_word_token(const char *line,
	int i, t_token **tokens, t_shell *shell)
{
	char	*value;
	int		next;

	next = extract_mixed_token(line, i, &value, shell);
	if (next == -1 || !value)
	{
		fprintf(stderr, "minishell: erro! aspas não fechadas ou overflow\n");
		return (i + 1);
	}
	add_token(tokens, new_token_with_expand(TOKEN_WORD, value, 1, 0));
	return (next);
}
