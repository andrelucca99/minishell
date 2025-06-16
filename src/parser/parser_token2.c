/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:30:16 by eschula           #+#    #+#             */
/*   Updated: 2025/06/16 19:03:01 by alucas-e         ###   ########.fr       */
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

void	finalize_command(
	t_command **cmds, t_command **cur, char *argv[], int argc)
{
	int		i;
	t_shell	*shell;

	shell = get_shell();
	(*cur)->args = gc_malloc(&shell->gc, sizeof(char *) * (argc + 1));
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
		argv[i++] = NULL;
	}
	(*cur)->args[argc] = NULL;
	add_command(cmds, *cur);
	*cur = new_command();
}
