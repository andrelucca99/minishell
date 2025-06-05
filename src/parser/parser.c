/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:52:09 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/05 14:04:40 by alucas-e         ###   ########.fr       */
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
		// t->type == TOKEN_HEREDOC significa que fizemos match em “<<”
		// o próximo token (t->next) será o delimitador, e esse token já carrega
		// o valor sem aspas em t->next->value, e sabe se t->next->expand é 0 ou 1.

		*tokens = t->next; // pula para o token do delimitador
		char *val = (*tokens)->value;      // e.g. "EOF" ou "EOF" sem aspa
		int expand_for_heredoc = (*tokens)->expand;
		cur->heredoc_delim = gc_strdup(val);
		cur->heredoc_expand  = expand_for_heredoc;
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

t_command *parse_tokens(t_token *tokens, t_shell *shell)
{
    t_command *cmds = NULL;
    t_command *cur = new_command();
    char *argv[MAX_ARGS] = {0};
    int argc = 0;

    while (tokens)
    {
        if (tokens->type == TOKEN_WORD)
        {
            // Se tiver expand=1, chamamos expand aqui; se for expand=0, pegamos
            // o valor literal sem modificação.
            char *val = tokens->expand
                ? expand_variables(tokens->value, shell)
                : tokens->value;
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

    return cmds;
}

int	extract_quoted_token(const char *line, int start, char quote, char **out)
{
	int	i = start + 1;

	while (line[i] && line[i] != quote)
		i++;
	if (!line[i])
		return (-1);
	if (out)
	    *out = gc_strndup(&line[start + 1], i - start - 1);
	return (i + 1);
}

static t_token *new_token_with_expand(t_token_type type, char *value, int expand)
{
	t_token *token;

	token = gc_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->expand = expand;
	token->next = NULL;
	return (token);
}

int process_token(const char *line, int i, t_token **tokens, t_shell *shell)
{
	t_token_type	type;
	int				op_len;
	int				start;
	char			*value;

	if (line[i] == '\'' || line[i] == '"')
	{
		// --- Caso de token entre aspas ---
		char quote = line[i];
		int next = extract_quoted_token(line, i, quote, &value);
		if (next == -1)
		{
			fprintf(stderr, "minishell: erro de aspas não fechadas\n");
			return i + 1;
		}

		// Se for aspas duplas → expandir
		if (quote == '"')
		{
			char *expanded = expand_variables(value, shell);
			add_token(tokens, new_token_with_expand(TOKEN_WORD, expanded, 0));  // já expandido
		}
		else
			add_token(tokens, new_token_with_expand(TOKEN_WORD, value, 0));  // sem expansão
		return next;
	}

	// --- Detecta operador: |, <, >, <<, >> ---
	op_len = 0;
	type = get_operator_type(&line[i], &op_len);
	if (type != TOKEN_WORD)
	{
		add_token(tokens, new_token_with_expand(type, gc_strndup(&line[i], op_len), 0));
		return i + op_len;
	}

	// --- Palavra simples (ex: EOF, $USER, abc) ---
	start = i;
	while (line[i] && !isspace(line[i]) && !ft_strchr("|<>", line[i]))
		i++;

	value = gc_strndup(&line[start], i - start);

	// ⚠️ Aqui é importante: como não veio entre aspas, marcamos expand = 1
	add_token(tokens, new_token_with_expand(TOKEN_WORD, value, 1));

	return i;
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
