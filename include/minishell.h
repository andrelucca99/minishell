/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:08:13 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/27 16:14:23 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include "../libs/libft/libft.h"

# define MAX_ARGS 128

extern char	**environ;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_command
{
	char				**args;
	char				*input_file;
	char				*output_file;
	int					append_mode;
	char				*heredoc_delim;
	struct s_command	*next;
}	t_command;

typedef struct s_gc_node
{
	void			*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node *head;
}	t_gc;

/* builtins */

int	is_builtin(char *cmd);
int	exec_builtin(char **args);

int	builtin_echo(char **args);
int	builtin_pwd(void);
int	builtin_env(void);
int	builtin_cd(char **args);
int	builtin_exit(void);

/* executor */
void	execute_commands(t_command *cmds);
char	*find_executable(char *cmd);
int		should_execute_builtin_in_parent(t_command *cmd);

/* parser / lexer */
t_token			*lexer(const char *line);
void			free_tokens(t_token *tokens);
t_command		*parse_tokens(t_token *tokens);
void			add_token(t_token **head, t_token *new);
t_token_type	get_operator_type(const char *s, int *len);
t_token			*new_token(t_token_type type, const char *value);
void			free_commands(t_command *cmds);
t_command		*new_command(void);
void			add_command(t_command **head, t_command *new);

/* signal */
void			handle_sigint(int signo);

/* garbage free */
void gc_clear(void);
char *gc_strndup(const char *s, size_t n);
char *gc_strdup(const char *s);
void *gc_malloc(size_t size);

void	gc_list(void);

#endif
