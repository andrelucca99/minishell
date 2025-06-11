/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:32:57 by eschula           #+#    #+#             */
/*   Updated: 2025/06/11 18:33:02 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_token(const char *line, int i, t_token **tokens, t_shell *shell)
{
	int	ret;

	if (line[i] == '\'' || line[i] == '"')
		return (handle_quoted_token(line, i, tokens, shell));
	ret = handle_operator_token(line, i, tokens);
	if (ret != -1)
		return (ret);
	if (line[i] && !isspace((unsigned char)
			line[i]) && !ft_strchr("|<>", line[i]))
		return (handle_word_token(line, i, tokens, shell));
	return (i + 1);
}

t_token	*lexer(const char *line, t_shell *shell)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (isspace(line[i]))
		{
			i++;
			continue ;
		}
		i = process_token(line, i, &tokens, shell);
	}
	return (tokens);
}
