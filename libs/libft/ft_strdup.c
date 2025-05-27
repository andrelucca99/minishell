/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:05:49 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/27 16:25:46 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*copy;

	copy = (char *)malloc(ft_strlen(str) + 1);
	if (!copy)
		return (NULL);
	ft_strcpy(copy, str);
	return (copy);
}
