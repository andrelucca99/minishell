/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <eschula@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 00:00:00 by emerson           #+#    #+#             */
/*   Updated: 2025/04/23 17:44:17 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * Inicializa a estrutura do shell
 */
void	init_shell(t_shell *shell, char **env)
{
	shell->env = dup_env(env);
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->last_exit_status = 0;
	shell->running = 1;
}

/**
 * Duplica o ambiente (env)
 */
char	**dup_env(char **env)
{
	char	**new_env;
	int		i;
	int		count;

	count = 0;
	while (env[count])
		count++;
	new_env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

/**
 * Limpa os recursos alocados pela estrutura do shell
 */
void	cleanup_shell(t_shell *shell)
{
	int	i;

	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->commands)
		free_commands(shell->commands);
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
}

/**
 * Função temporária para liberar tokens
 */
void	free_tokens(t_token *tokens)
{
	// Implementação será feita posteriormente
	(void)tokens;
}

/**
 * Função temporária para liberar comandos
 */
void	free_commands(t_command *commands)
{
	// Implementação será feita posteriormente
	(void)commands;
}

/**
 * Loop principal do shell
 */
void	run_shell(t_shell *shell)
{
	char	*input;
	char	prompt[100];

	while (shell->running)
	{
		// Cria um prompt colorido
		snprintf(prompt, sizeof(prompt), "%sminishell%s $ %s", CYAN, GREEN, RESET);
		input = readline(prompt);

		// Verifica se o usuário digitou Ctrl+D (EOF)
		if (!input)
		{
			printf("exit\n");
			break;
		}

		// Adiciona o comando ao histórico se não for vazio
		if (*input)
			add_history(input);

		// TODO: Implementar análise léxica, parsing e execução
		printf("Comando digitado: %s\n", input);

		// Se o usuário digitar "exit", sair do loop
		if (strcmp(input, "exit") == 0)
			shell->running = 0;

		free(input);
	}
}

/**
 * Função principal
 */
int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	// Configurar tratamento de sinais
	// setup_signals();  // Será implementado posteriormente

	// Inicializar a estrutura do shell
	init_shell(&shell, env);

	// Executar o loop principal do shell
	run_shell(&shell);

	// Limpar recursos
	cleanup_shell(&shell);

	return (shell.last_exit_status);
}