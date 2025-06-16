/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:12:01 by eschula           #+#    #+#             */
/*   Updated: 2025/06/16 19:03:54 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_token_word(
	t_token *tokens, t_shell *shell, char **argv, int *argc)
{
	char	*val;

	if (tokens->expand)
		val = expand_variables(tokens->value, shell);
	else
		val = tokens->value;
	argv[(*argc)++] = gc_strdup(val);
}

void	handle_token_pipe(
	t_command **cmds, t_command **cur, char **argv, int *argc)
{
	finalize_command(cmds, cur, argv, *argc);
	*argc = 0;
}

void	handle_token_else(
	t_token **tokens, t_command *cur)
{
	handle_redirection(tokens, cur);
}

char	*ft_strjoin_g(const char *s1, const char *s2)
{
	size_t	l1;
	size_t	l2;
	t_shell	*shell;
	char	*res;

	l1 = 0;
	l2 = 0;
	shell = get_shell();
	if (s2 == NULL)
		return (NULL);
	if (s1)
		l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	res = gc_malloc(&shell->gc, l1 + l2 + 1);
	if (!res)
		return (NULL);
	if (s1)
		ft_memmove(res, s1, l1);
	ft_memmove(res + l1, s2, l2 + 1);
	return (res);
}

char	*ft_strjoin_g_len(const char *s1, const char *s2, size_t len2)
{
	size_t	l1;
	char	*res;
	t_shell	*shell;

	shell = get_shell();
	if (s1)
		l1 = ft_strlen(s1);
	else
		l1 = 0;
	res = gc_malloc(&shell->gc, l1 + len2 + 1);
	if (!res)
		return (NULL);
	if (s1)
		ft_memcpy(res, s1, l1);
	ft_memcpy(res + l1, s2, len2);
	res[l1 + len2] = '\0';
	return (res);
}
