/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:09:33 by alucas-e          #+#    #+#             */
/*   Updated: 2025/05/09 17:16:10 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

char	*ft_strtok(char *str, const char *delim)
{
	static char	*save;
	char		*start;

	if (str)
		save = str;
	if (!save || !*save)
		return (NULL);
	while (*save && ft_strchr(delim, *save))
		save++;
	if (!*save)
		return (NULL);
	start = save;
	while (*save && !ft_strchr(delim, *save))
		save++;
	if (*save)
	{
		*save = '\0';
		save++;
	}
	return (start);
}
