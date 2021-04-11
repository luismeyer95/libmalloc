/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_itoa.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/10 11:38:18 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/20 12:37:20 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static void		ft_putchar_str(char c, char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	str[i] = c;
	str[i + 1] = '\0';
	return ;
}

static int		get_slen(int n)
{
	int slen;

	if (!n)
		return (2);
	slen = 1;
	if (n < 0)
		slen++;
	while (n)
	{
		n /= 10;
		slen++;
	}
	return (slen);
}

static void		ft_itoa_bis(unsigned int abs, char *str)
{
	unsigned int	pop_unit;

	if (abs < 10)
		ft_putchar_str(abs + '0', str);
	else
	{
		pop_unit = abs % 10;
		abs /= 10;
		ft_itoa_bis(abs, str);
		ft_putchar_str(pop_unit + '0', str);
	}
	return ;
}

char			*ft_itoa(int n)
{
	unsigned int	abs;
	unsigned int	slen;
	char			*str;

	slen = get_slen(n);
	if (!(str = malloc(slen * sizeof(char))))
		return (NULL);
	*str = 0;
	if (n < 0)
		ft_putchar_str('-', str);
	abs = n < 0 ? -n : n;
	ft_itoa_bis(abs, str);
	return (str);
}
