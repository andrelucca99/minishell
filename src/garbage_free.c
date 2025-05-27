/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:22:14 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/27 16:44:40 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_gc g_gc = { NULL };

void *gc_malloc(size_t size)
{
	void *ptr = malloc(size);
	if (!ptr)
		return (NULL);
	t_gc_node *node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = g_gc.head;
	g_gc.head = node;
	return (ptr);
}

char *gc_strdup(const char *s)
{
	size_t len = ft_strlen(s);
	char *dup = gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	ft_strcpy(dup, s);
	return (dup);
}

char *gc_strndup(const char *s, size_t n)
{
	size_t len = 0;
	while (s[len] && len < n)
		len++;
	char *dup = gc_malloc(len + 1);
	if (!dup)
		return (NULL);
	for (size_t i = 0; i < len; i++)
		dup[i] = s[i];
	dup[len] = '\0';
	return (dup);
}

void gc_clear(void)
{
	t_gc_node **cur = &g_gc.head;

	while (*cur)
	{
		t_gc_node *next = (*cur)->next;
		free((*cur)->ptr);
		free(*cur);
		*cur = next;
	}
}
