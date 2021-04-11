/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strchr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/08 13:36:24 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/10 16:20:44 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	size_t	len;
	char	*src;

	len = 0;
	src = (char*)s;
	while (src[len])
		len++;
	if (c == '\0')
		return (src + len);
	i = 0;
	while (src[i] != c && src[i])
		i++;
	if (i == len)
		return (0);
	else
		return (src + i);
}
