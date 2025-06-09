/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:49:54 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/02 15:30:45 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = gc_malloc(sizeof(t_command));
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

/* rafatorar depois */

char	*expand_variables(const char *str, t_shell *shell)
{
	char	buffer[4096];
	char	num[12];
	char	*name;
	char	*value;
	int		start;
	int		k;
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
				sprintf(num, "%d", shell->last_exit_status);
				k = 0;
				while (num[k])
					buffer[j++] = num[k++];
				i += 2;
			}
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
			{
				start = ++i;
				while (ft_isalnum(str[i]) || str[i] == '_')
					i++;
				name = gc_strndup(&str[start], i - start);
				value = getenv(name);
				if (value)
				{
					k = 0;
					while (value[k])
						buffer[j++] = value[k++];
				}
			}
			else
				buffer[j++] = str[i++];
		}
		else
			buffer[j++] = str[i++];
	}
	buffer[j] = '\0';
	return (gc_strdup(buffer));
}
