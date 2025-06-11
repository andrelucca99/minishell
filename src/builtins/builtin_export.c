/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:25:22 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/11 19:16:26 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	list_exports(t_shell *shell)
{
	int	j;

	j = 0;
	while (shell->env[j])
	{
		printf("declare -x %s\n", shell->env[j]);
		j++;
	}
	shell->last_exit_status = 0;
	return (0);
}

static int	validate_and_export_empty(char *opt, t_shell *shell)
{
	int	k;

	k = 1;
	if (!(ft_isalpha(opt[0]) || opt[0] == '_'))
	{
		fprintf(stderr, "export: `%s`: not a valid identifier\n", opt);
		shell->last_exit_status = 1;
		return (1);
	}
	while (opt[k])
	{
		if (!(ft_isalnum(opt[k]) || opt[k] == '_'))
		{
			fprintf(stderr, "export: `%s`: not a valid identifier\n", opt);
			shell->last_exit_status = 1;
			return (1);
		}
		k++;
	}
	if (setenv(opt, "", 1) != 0)
		perror("export");
	return (0);
}

static int	export_with_value(char *opt, t_shell *shell)
{
	char	*name;
	char	*rawval;
	char	*value;
	char	*equal;

	equal = ft_strchr(opt, '=');
	name = gc_strndup(opt, equal - opt);
	rawval = equal + 1;
	value = expand_variables(rawval, shell);
	if (setenv(name, value, 1) != 0)
		perror("export");
	shell->last_exit_status = 0;
	return (0);
}

int	builtin_export(char **args, t_shell *shell)
{
	int		i;
	char	*opt;
	char	*equal;

	i = 1;
	if (!args[i])
		return (list_exports(shell));
	while (args[i])
	{
		opt = args[i];
		equal = ft_strchr(opt, '=');
		if (!equal)
		{
			if (validate_and_export_empty(opt, shell) != 0)
				return (1);
		}
		else
		{
			if (export_with_value(opt, shell) != 0)
				return (1);
		}
		i++;
	}
	shell->last_exit_status = 0;
	return (0);
}
