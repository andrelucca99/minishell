/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:49:54 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/13 15:01:02 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;
	t_shell		*shell;

	shell = get_shell();
	cmd = gc_malloc(&shell->gc, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_expand = 0;
	cmd->next = NULL;
	return (cmd);
}

void	add_command(t_command **head, t_command *new)
{
	t_command	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

static int	append_exit_status(char *buffer, int j, t_shell *shell)
{
	char	num[12];
	int		k;

	sprintf(num, "%d", shell->last_exit_status);
	k = 0;
	while (num[k])
		buffer[j++] = num[k++];
	return (j);
}

static int	append_env_var(const char *str, int *i, char *buffer, int j)
{
	int		start;
	int		k;
	char	*name;
	char	*value;

	start = ++(*i);
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	name = gc_strndup(&str[start], *i - start);
	value = getenv(name);
	if (value)
	{
		k = 0;
		while (value[k])
			buffer[j++] = value[k++];
	}
	return (j);
}

char	*expand_variables(const char *str, t_shell *shell)
{
	char	buffer[4096];
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i] && j < 4090)
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				j = append_exit_status(buffer, j, shell);
				i += 2;
			}
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
				j = append_env_var(str, &i, buffer, j);
			else
				buffer[j++] = str[i++];
		}
		else
			buffer[j++] = str[i++];
	}
	buffer[j] = '\0';
	return (gc_strdup(buffer));
}
