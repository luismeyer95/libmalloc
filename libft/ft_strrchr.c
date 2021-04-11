/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strrchr.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/08 14:06:39 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/15 14:06:03 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	int		len;
	char	*src;

	len = 0;
	src = (char*)s;
	while (src[len])
		len++;
	if ((char)c == '\0')
		return (src + len);
	i = len;
	while (i >= 0 && src[i] != (char)c)
		i--;
	if (i == -1)
		return (0);
	else
		return (src + i);
}
