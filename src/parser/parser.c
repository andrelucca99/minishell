/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:52:09 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/27 16:19:00 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_redirection(t_token **tokens, t_command *cur)
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
	{
		*tokens = t->next;
		cur->heredoc_delim = gc_strdup((*tokens)->value);
	}
}

static void	finalize_command(
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

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmds;
	t_command	*cur;
	char		*argv[MAX_ARGS] = {0};
	int			argc;

	cmds = NULL;
	cur = new_command();
	argc = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
			argv[argc++] = gc_strdup(tokens->value);
		else if (tokens->type == TOKEN_PIPE)
		{
			finalize_command(&cmds, &cur, argv, argc);
			argc = 0;
		}
		else
			handle_redirection(&tokens, cur);
		tokens = tokens->next;
	}
	if (argc > 0)
		finalize_command(&cmds, &cur, argv, argc);
	else if (cur && cur->args == NULL)
		free(cur);
	else
		add_command(&cmds, cur);
	return (cmds);
}

int	process_token(const char *line, int i, t_token **tokens)
{
	t_token_type	type;
	int				op_len;
	int				start;

	op_len = 0;
	type = get_operator_type(&line[i], &op_len);
	if (type != TOKEN_WORD)
	{
		add_token(tokens, new_token(type, gc_strndup(&line[i], op_len)));
		return (i + op_len);
	}
	else
	{
		start = i;
		while (line[i] && !isspace(line[i]) && !ft_strchr("|<>", line[i]))
			i++;
		add_token(tokens,
			new_token(TOKEN_WORD, gc_strndup(&line[start], i - start)));
		return (i);
	}
}

t_token	*lexer(const char *line)
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
		i = process_token(line, i, &tokens);
	}
	return (tokens);
}
