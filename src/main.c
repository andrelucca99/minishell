/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:56:29 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/19 19:19:08 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_str_is_empty(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	run_shell_loop(t_shell *shell)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmds;

	while (shell->running)
	{
		signal(SIGINT, handle_sigint);
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (ft_str_is_empty(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		tokens = lexer(line, shell);
		cmds = parse_tokens(tokens, shell);
		execute_commands(cmds, shell);
		free(line);
		gc_clear(&shell->gc);
	}
}

int	main(void)
{
	t_shell	*shell;

	shell = get_shell();
	shell->gc.head = NULL;
	shell->env = environ;
	shell->last_exit_status = 0;
	shell->running = 1;
	signal(SIGQUIT, SIG_IGN);
	run_shell_loop(shell);
	return (shell->last_exit_status);
}
