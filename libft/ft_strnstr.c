/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strnstr.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/08 15:36:30 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/11 14:18:15 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	count;
	size_t	j;
	size_t	nlen;

	nlen = 0;
	i = -1;
	count = 0;
	if (*needle == '\0')
		return ((char*)haystack);
	while (needle[nlen])
		nlen++;
	while (haystack[++i] && i < len)
		if (haystack[i] == needle[0])
		{
			j = -1;
			count = 0;
			while (haystack[i + ++j] && needle[j] && i + j < len)
				if (haystack[i + j] == needle[j])
					count++;
			if (count == nlen)
				return ((char*)haystack + i);
		}
	return (0);
}
