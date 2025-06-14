/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschula <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:22:14 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/14 14:30:33 by eschula          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*gc_malloc(t_gc *gc, size_t size)
{
	void		*ptr;
	t_gc_node	*node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = gc->head;
	gc->head = node;
	return (ptr);
}

char	*gc_strdup(const char *s)
{
	size_t	len;
	char	*dup;
	t_shell	*shell;

	shell = get_shell();
	len = ft_strlen(s);
	dup = gc_malloc(&shell->gc, len + 1);
	if (!dup)
		return (NULL);
	ft_strcpy(dup, s);
	return (dup);
}

char	*gc_strndup(const char *s, size_t n)
{
	size_t	len;
	size_t	i;
	char	*dup;
	t_shell	*shell;

	shell = get_shell();
	len = 0;
	while (s[len] && len < n)
		len++;
	dup = gc_malloc(&shell->gc, len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

void	gc_clear(t_gc *gc)
{
	t_gc_node	**cur;
	t_gc_node	*next;

	cur = &gc->head;
	while (*cur)
	{
		next = (*cur)->next;
		free((*cur)->ptr);
		free(*cur);
		*cur = next;
	}
}
