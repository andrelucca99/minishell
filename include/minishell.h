/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 00:00:00 by emerson           #+#    #+#             */
/*   Updated: 2025/04/23 17:42:05 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Definições de cores para o prompt */
# define CYAN "\001\033[1;36m\002"
# define GREEN "\001\033[1;32m\002"
# define RESET "\001\033[0m\002"

/* Definições de códigos de saída */
# define SUCCESS 0
# define ERROR 1

/* Estruturas de dados */

/* Estrutura para tokens do lexer */
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF
} t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
} t_token;

/* Estrutura para comandos após o parsing */
typedef struct s_command
{
    char            **args;
    char            *input_file;
    char            *output_file;
    int             append_mode;
    char            *heredoc_delim;
    struct s_command *next;
} t_command;

/* Estrutura principal do shell */
typedef struct s_shell
{
    char            **env;
    t_token         *tokens;
    t_command       *commands;
    int             last_exit_status;
    int             running;
} t_shell;

/* Protótipos de funções */

/* Inicialização e principais */
void    init_shell(t_shell *shell, char **env);
void    run_shell(t_shell *shell);
void    cleanup_shell(t_shell *shell);

/* Funções do lexer */
t_token *tokenize(char *input);
void    free_tokens(t_token *tokens);

/* Funções do parser */
t_command *parse_tokens(t_token *tokens);
void    free_commands(t_command *commands);

/* Funções do expander */
void    expand_variables(t_command *commands, char **env, int last_exit_status);

/* Funções do executor */
void    execute_commands(t_command *commands, t_shell *shell);

/* Funções de builtin */
int     is_builtin(char *cmd);
int     execute_builtin(char **args, t_shell *shell);
int     builtin_echo(char **args);
int     builtin_cd(char **args, t_shell *shell);
int     builtin_pwd(void);
int     builtin_export(char **args, t_shell *shell);
int     builtin_unset(char **args, t_shell *shell);
int     builtin_env(t_shell *shell);
int     builtin_exit(char **args, t_shell *shell);

/* Funções de manipulação de sinais */
void    setup_signals(void);

/* Funções utilitárias */
char    **dup_env(char **env);
char    *get_env_value(char **env, char *name);
void    set_env_value(char ***env, char *name, char *value);
void    unset_env_value(char ***env, char *name);

#endif