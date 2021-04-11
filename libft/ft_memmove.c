/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memmove.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/08 11:01:28 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/11 21:02:29 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const char	*src2;
	char		*dst2;
	size_t		i;
	size_t		k;

	if (!dst && !src)
		return (NULL);
	i = 0;
	k = len;
	src2 = src;
	dst2 = dst;
	while (i < len)
	{
		if (src2 + i == dst2)
		{
			while (k >= 1)
			{
				dst2[k - 1] = src2[k - 1];
				k--;
			}
			return (dst);
		}
		i++;
	}
	return (ft_memcpy(dst, src, len));
}
