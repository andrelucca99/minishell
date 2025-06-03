/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buitin_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:25:22 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/03 15:48:14 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_export(char **args, t_shell *shell)
{
	int i = 1;
	int j;

	if (!args[1])
	{
		// Sem argumentos: listar variáveis (como bash)
		j = 0;
		while (shell->env[j])
		{
			printf("declare -x %s\n", shell->env[j]);
		    j++;
		}
	    return (0);
	}

	while (args[i])
	{
		char *expanded = expand_variables(args[i], shell);
		char *equal = ft_strchr(expanded, '=');

		if (!equal)
		{
			fprintf(stderr, "export: `%s`: not a valid identifier\n", expanded);
			i++;
			continue;
		}

		// Divide em nome e valor
		*equal = '\0';
		char *name = expanded;
		char *value = equal + 1;

		if (setenv(name, value, 1) != 0)
			perror("export");

		i++;
	}
	return (0);
}