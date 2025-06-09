/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:56:29 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/05 14:27:37 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
	char		*line;
	t_token		*tokens;
	t_command	*cmds;
	t_shell		shell;

	shell.env = environ;
	shell.last_exit_status = 0;
	shell.running = 1;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);

	while (shell.running)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = lexer(line, &shell);
		cmds = parse_tokens(tokens, &shell);
		execute_commands(cmds, &shell);
		free(line);
		gc_clear();
	}
	return (shell.last_exit_status);
}
