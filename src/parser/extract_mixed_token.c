/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_mixed_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:06:16 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/09 15:48:09 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_strjoin_g(const char *s1, const char *s2)
{
	size_t	l1;
	size_t	l2;
	char	*res;

	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	res = gc_malloc(l1 + l2 + 1);
	ft_memmove(res, s1, l1);
	ft_memmove(res + l1, s2, l2 + 1);
	return (res);
}

int	extract_mixed_token(const char *line, int i, char **out, t_shell *shell)
{
	char	*expanded;
	char	*result;
	char	*part;
	char	*raw;
	int		j;

	result = gc_strdup(""); // string inicial vazia
	while (line[i] && !isspace((unsigned char)line[i])
		&& !ft_strchr("|<>", line[i]))
	{
		if (line[i] == '\'') // aspas simples: copia literal
		{
			j = i + 1;
			while (line[j] && line[j] != '\'')
				j++;
			if (line[j] != '\'')
			{
				*out = NULL;
				return (-1);
			}
			part = gc_strndup(&line[i + 1], j - i - 1);
			result = ft_strjoin_g(result, part);
			i = j + 1;
		}
		else if (line[i] == '"') // aspas duplas: copia e expande
		{
			j = i + 1;
			while (line[j] && line[j] != '"')
				j++;
			if (line[j] != '"')
			{
				*out = NULL;
				return (-1);
			}
			raw = gc_strndup(&line[i + 1], j - i - 1);
			expanded = expand_variables(raw, shell);
			result = ft_strjoin_g(result, expanded);
			i = j + 1;
		}
		else // texto normal: copia até espaço, pipe ou aspas
		{
			j = i;
			while (line[j] && !isspace(
					(unsigned char)line[j]) && !ft_strchr("|<>'\"", line[j]))
				j++;
			part = gc_strndup(&line[i], j - i);
			result = ft_strjoin_g(result, part);
			i = j;
		}
	}
	*out = result;
	return (i);
}
