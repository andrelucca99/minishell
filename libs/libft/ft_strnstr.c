/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:36:27 by alucas-e          #+#    #+#             */
/*   Updated: 2024/10/17 11:09:22 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	len;

	len = 0;
	while (to_find[len] != '\0')
		len++;
	if (len == 0)
		return ((char *)str);
	i = 0;
	while (str[i] != '\0' && i < size)
	{
		if (str[i] == to_find[0])
		{
			j = 0;
			while (str[i + j] == to_find[j] && (i + j) < size)
			{
				if (to_find[j + 1] == '\0')
					return ((char *)&str[i]);
				j++;
			}
		}
		i++;
	}
	return (NULL);
}
