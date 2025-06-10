/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:52:09 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/10 17:36:39 by alucas-e         ###   ########.fr       */
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
		if ((*tokens)->was_quoted)
			cur->heredoc_expand = 0;
		else
			cur->heredoc_expand = 1;
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

t_command	*parse_tokens(t_token *tokens, t_shell *shell)
{
	t_command	*cmds;
	t_command	*cur;
	char		*argv[MAX_ARGS];
	char		*val;
	int			argc;

	cmds = NULL;
	cur = new_command();
	argc = 0;
	ft_bzero(argv, sizeof(argv));
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if (tokens->expand)
				val = expand_variables(tokens->value, shell);
			else
				val = tokens->value;
			argv[argc++] = gc_strdup(val);
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			finalize_command(&cmds, &cur, argv, argc);
			argc = 0;
		}
		else
		{
			// Redirecionamentos (<, >, >>, <<)
			handle_redirection(&tokens, cur);
		}
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

static t_token	*new_token_with_expand(t_token_type type,
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

int	process_token(const char *line, int i, t_token **tokens, t_shell *shell)
{
	t_token_type	type;
	char			*expanded;
	char			*value;
	char			quote;
	char			*sym;
	int				op_len;
	int				next;

	// 1) Quoted token: aspas simples ou duplas
	if (line[i] == '\'' || line[i] == '"')
	{
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
			add_token(tokens,
				new_token_with_expand(TOKEN_WORD, expanded, 0, 1));
		}
		else
		{
			add_token(tokens,
				new_token_with_expand(TOKEN_WORD, value, 0, 1));
		}
		return (next);
	}
	// 2) Operadores: |, <, >, >>, <<
	type = get_operator_type(&line[i], &op_len);
	if (type != TOKEN_WORD)
	{
		sym = gc_strndup(&line[i], op_len);
		add_token(tokens,
			new_token_with_expand(type, sym, 0, 0));
		return (i + op_len);
	}
	// 3) Palavra ou token misto (texto + '...' + "...")
	if (line[i] && !isspace((unsigned char)line[i])
		&& !ft_strchr("|<>", line[i]))
	{
		next = extract_mixed_token(line, i, &value, shell);
		if (next == -1 || !value)
		{
			fprintf(stderr, "minishell: erro! aspas não fechadas ou overflow\n");
			return (i + 1);
		}
		// value já vem com "..." expandidas e '...' literais
		add_token(tokens,
			new_token_with_expand(TOKEN_WORD, value, 1, 0));
		return (next);
	}
	// 4) Espaço ou caractere inválido: consome e avança
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
