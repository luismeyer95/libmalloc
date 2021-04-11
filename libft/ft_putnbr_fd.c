/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_putnbr_fd.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/10 14:26:49 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/20 12:43:33 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static void	loopwrite(unsigned int abs, int fd)
{
	unsigned int	pop_unit;

	if (abs < 10)
		ft_putchar_fd(abs + '0', fd);
	else
	{
		pop_unit = abs % 10;
		abs /= 10;
		loopwrite(abs, fd);
		ft_putchar_fd(pop_unit + '0', fd);
	}
	return ;
}

void		ft_putnbr_fd(int n, int fd)
{
	unsigned int	abs;

	if (n < 0)
		ft_putchar_fd('-', fd);
	abs = n < 0 ? 0 - n : n;
	loopwrite(abs, fd);
	return ;
}
