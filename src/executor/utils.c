/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:30:34 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/03 16:14:13 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*find_executable(char *cmd)
{
	char	*path;
	char	*paths;
	char	*token;
	char	fullpath[1024];

	if (ft_strchr(cmd, '/'))
		return (gc_strdup(cmd));
	path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = gc_strdup(path);
	token = ft_strtok(paths, ":");
	while (token)
	{
		snprintf(fullpath, sizeof(fullpath), "%s/%s", token, cmd);
		if (access(fullpath, X_OK) == 0)
		{
			free(paths);
			return (gc_strdup(fullpath));
		}
		token = ft_strtok(NULL, ":");
	}
	free(paths);
	return (NULL);
}

int	should_execute_builtin_in_parent(t_command *cmd)
{
	return (!cmd->next && is_builtin(cmd->args[0])
		&& !cmd->input_file && !cmd->output_file);
}

int handle_heredoc(const char *delim, int expand, t_shell *shell)
{
	int fd[2];
	char *line;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			char *expanded = expand_variables(line, shell);
			write(fd[1], expanded, ft_strlen(expanded));
			write(fd[1], "\n", 1);
			free(expanded);
		}
		else
		{
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
		}
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}
