/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luis <luis@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 18:09:12 by lumeyer           #+#    #+#             */
/*   Updated: 2021/04/11 15:37:36 by luis             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"

static int		ft_strlen_mod(char const *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

static size_t	get_wc(char const *s, char c)
{
	size_t	wcount;
	size_t	i;

	if (!(*s))
		return (0);
	i = 1;
	wcount = (c == s[0]) ? 0 : 1;
	while (s[i])
	{
		if (s[i] != c && s[i - 1] == c)
			wcount++;
		i++;
	}
	return (wcount);
}

char			**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	wc;
	char	**mat;

	if (!s)
		return (NULL);
	i = 0;
	wc = get_wc(s, c);
	if (!(mat = (char**)malloc((wc + 1) * sizeof(char*))))
		return (NULL);
	k = 0;
	while (wc--)
	{
		while (s[k] && s[k] == c)
			k++;
		j = 0;
		mat[i] = (char*)malloc((ft_strlen_mod(s + k, c) + 1) * sizeof(char));
		while (s[k] && s[k] != c)
			mat[i][j++] = s[k++];
		mat[i++][j] = '\0';
	}
	mat[i] = 0;
	return (mat);
}
