/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:25:22 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/11 14:06:56 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	builtin_export(char **args, t_shell *shell)
// {
// 	int		i;
// 	int		j;
// 	char	*expanded;
// 	char	*equal;
// 	char	*name;
// 	char	*value;

// 	i = 1;
// 	if (!args[1])
// 	{
// 		// Sem argumentos: listar variáveis (como bash)
// 		j = 0;
// 		while (shell->env[j])
// 		{
// 			printf("declare -x %s\n", shell->env[j]);
// 			j++;
// 		}
// 		return (0);
// 	}
// 	while (args[i])
// 	{
// 		expanded = expand_variables(args[i], shell);
// 		equal = ft_strchr(expanded, '=');
// 		if (!equal)
// 		{
// 			fprintf(stderr, "export: `%s`: not a valid identifier\n", expanded);
// 			i++;
// 			continue ;
// 		}
// 		// Divide em nome e valor
// 		*equal = '\0';
// 		name = expanded;
// 		value = equal + 1;
// 		if (setenv(name, value, 1) != 0)
// 			perror("export");
// 		i++;
// 	}
// 	return (0);
// }

int builtin_export(char **args, t_shell *shell)
{
    int i = 1;

    if (!args[i])
    {
        // Lista variáveis
        for (int j = 0; shell->env[j]; j++)
            printf("declare -x %s\n", shell->env[j]);
        shell->last_exit_status = 0;
        return 0;
    }

    while (args[i])
{
    char *opt = args[i];
    char *eq  = ft_strchr(opt, '=');

    if (!eq)
    {
        // valida primeiro caractere
        if (!(ft_isalpha(opt[0]) || opt[0] == '_'))
            goto invalid;
        // valida demais caracteres
        for (int k = 1; opt[k]; k++)
            if (!(ft_isalnum(opt[k]) || opt[k] == '_'))
                goto invalid;

        // nome ok: exporta com valor vazio
        if (setenv(opt, "", 1) != 0)
            perror("export");
        i++;
        continue;
    }

    // ... resto do export com “=” ...

    i++;
    continue;

invalid:
    fprintf(stderr, "export: `%s': not a valid identifier\n", opt);
    shell->last_exit_status = 1;
    return 1;
}


    shell->last_exit_status = 0;
    return 0;
}

