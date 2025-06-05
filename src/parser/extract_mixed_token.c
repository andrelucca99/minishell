/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_mixed_token.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alucas-e <alucas-e@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 14:06:16 by alucas-e          #+#    #+#             */
/*   Updated: 2025/06/05 14:06:18 by alucas-e         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *ft_strjoin_g(const char *s1, const char *s2)
{
    size_t len1 = ft_strlen(s1);
    size_t len2 = ft_strlen(s2);
    char *res  = gc_malloc(len1 + len2 + 1);
    if (!res)
        return NULL;
    ft_memmove(res, s1, len1);
    ft_memmove(res + len1, s2, len2);
    res[len1 + len2] = '\0';
    return res;
}

int extract_mixed_token(const char *line, int start, char **out, t_shell *shell)
{
    int   i = start;
    char *result = gc_strdup("");  // string “vazia” alocada pelo GC

    while (line[i])
    {
        // 1) Se espaço ou metacaractere, encerra o token
        if (isspace((unsigned char)line[i]) || ft_strchr("|<>", line[i]))
            break;

        // 2) Aspas simples: copia literalmente o interior, sem expansão
        if (line[i] == '\'')
        {
            int j = i + 1;
            while (line[j] && line[j] != '\'')
                j++;
            if (line[j] != '\'')
            {
                // aspas não fechada: falha
                *out = NULL;
                return -1;
            }
            int len_inside = j - (i + 1);
            // duplica conteúdo interno
            char *piece = gc_strndup(&line[i + 1], len_inside);
            // junta em result
            char *tmp = ft_strjoin_g(result, piece);
            result = tmp;
            i = j + 1;
            continue;
        }

        // 3) Aspas duplas: copia interior e expande
        if (line[i] == '"')
        {
            int j = i + 1;
            while (line[j] && line[j] != '"')
                j++;
            if (line[j] != '"')
            {
                // aspas não fechada
                *out = NULL;
                return -1;
            }
            int len_inside = j - (i + 1);
            // duplica interior
            char *inner = gc_strndup(&line[i + 1], len_inside);
            // expande variáveis dentro das duplas
            char *expanded = expand_variables(inner, shell);
            // concatena em result
            char *tmp = ft_strjoin_g(result, expanded);
            result = tmp;
            i = j + 1;
            continue;
        }

        // 4) Texto “normal” sem aspas: copia até espaço ou metacaractere
        int j = i;
        while (line[j] && !isspace((unsigned char)line[j]) && !ft_strchr("|<>\"'", line[j]))
            j++;
        int len = j - i;
        if (len > 0)
        {
            char *piece = gc_strndup(&line[i], len);
            char *tmp   = ft_strjoin_g(result, piece);
            result = tmp;
        }
        i = j;
    }

    *out = result;
    return i;
}

