/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:02:51 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/16 15:56:41 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	size_t	i;
	char	*copy;

	len = 0;
	while (s[len] && len < n)
		len++;
	copy = malloc(len + 1);
	if (!copy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = s[i];
		i++;
	}
	copy[len] = '\0';
	return (copy);
}
