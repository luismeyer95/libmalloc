/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memccpy.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/08 11:02:57 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/11 20:08:14 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char		*src2;
	unsigned char		*dst2;

	src2 = (unsigned char*)src;
	dst2 = (unsigned char*)dst;
	if (!dst && !src)
		return (NULL);
	while (n--)
	{
		*dst2 = *src2;
		if (*src2 == (unsigned char)c)
			return ((void*)++dst2);
		dst2++;
		src2++;
	}
	return (NULL);
}
