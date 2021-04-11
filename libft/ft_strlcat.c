/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strlcat.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/09 13:02:34 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/20 13:15:36 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t i;
	size_t j;
	size_t dlength;
	size_t slength;

	dlength = ft_strlen(dst);
	slength = ft_strlen(src);
	i = 0;
	j = 0;
	if (size == 0 || dlength >= size)
		return (size + slength);
	while (dst[i] != '\0')
		i++;
	while (j < size - dlength - 1 && src[j] != '\0')
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dlength + slength);
}
