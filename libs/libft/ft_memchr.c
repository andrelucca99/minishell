/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 17:33:54 by alucas-e          #+#    #+#             */
/*   Updated: 2024/10/18 16:24:45 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	unsigned char		byte;
	const unsigned char	*ptr;

	ptr = (unsigned char *)s;
	byte = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == byte)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}
