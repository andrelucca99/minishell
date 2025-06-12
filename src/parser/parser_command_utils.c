/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:12:01 by eschula           #+#    #+#             */
/*   Updated: 2025/06/12 19:07:40 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	finalize_command(
	t_command **cmds, t_command **cur, char *argv[], int argc)
{
	int	i;

	(*cur)->args = gc_malloc(sizeof(char *) * (argc + 1));
	if (!(*cur)->args)
	{
		i = 0;
		while (i < argc)
		{
			free(argv[i]);
			argv[i] = NULL;
			i++;
		}
		return ;
	}
	i = 0;
	while (i < argc)
	{
		(*cur)->args[i] = argv[i];
		argv[i] = NULL;
		i++;
	}
	(*cur)->args[argc] = NULL;
	add_command(cmds, *cur);
	*cur = new_command();
}

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
	char	*res;

	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	res = gc_malloc(l1 + l2 + 1);
	ft_memmove(res, s1, l1);
	ft_memmove(res + l1, s2, l2 + 1);
	return (res);
}
