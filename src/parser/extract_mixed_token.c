/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_mixed_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:06:16 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/16 18:15:40 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_single_quote(const char *line, int i, char **result)
{
	int		j;
	char	*part;

	j = i + 1;
	while (line[j] && line[j] != '\'')
		j++;
	if (line[j] != '\'')
		return (-1);
	part = gc_strndup(&line[i + 1], j - i - 1);
	*result = ft_strjoin_g(*result, part);
	return (j + 1);
}

static int	handle_double_quote(const char *line,
	int i, char **result, t_shell *shell)
{
	int		j;
	char	*raw;
	char	*expanded;

	j = i + 1;
	while (line[j] && line[j] != '"')
		j++;
	if (line[j] != '"')
		return (-1);
	raw = gc_strndup(&line[i + 1], j - i - 1);
	expanded = expand_variables(raw, shell);
	*result = ft_strjoin_g(*result, expanded);
	return (j + 1);
}

static int	handle_plain_text(const char *line, int i, char **result)
{
	int		j;

	j = i;
	while (line[j] && !isspace((unsigned char)line[j])
		&& !ft_strchr("|<>'\"", line[j]))
		j++;
	*result = ft_strjoin_g_len(*result, &line[i], j - i);
	return (j);
}

static int	process_token_piece(const char *line,
	int *i, char **result, t_shell *shell)
{
	int	next;

	if (line[*i] == '\'')
	{
		next = handle_single_quote(line, *i, result);
		if (next == -1)
			return (-1);
		*i = next;
	}
	else if (line[*i] == '"')
	{
		next = handle_double_quote(line, *i, result, shell);
		if (next == -1)
			return (-1);
		*i = next;
	}
	else
		*i = handle_plain_text(line, *i, result);
	return (0);
}

int	extract_mixed_token(const char *line, int i, char **out, t_shell *shell)
{
	char	*result;

	result = NULL;
	while (line[i] && !isspace((unsigned char)line[i])
		&& !ft_strchr("|<>", line[i]))
	{
		if (process_token_piece(line, &i, &result, shell) == -1)
		{
			*out = NULL;
			return (-1);
		}
	}
	*out = result;
	return (i);
}
