/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_calloc.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/08 17:43:52 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/16 18:27:11 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void			*tab;
	size_t			tsize;

	tsize = count * size;
	if (!(tab = malloc(tsize)))
		return (NULL);
	ft_memset(tab, 0, tsize);
	return (tab);
}
