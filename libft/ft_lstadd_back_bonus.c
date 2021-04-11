/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstadd_back_bonus.c                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/11 15:24:44 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/17 11:09:51 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *current;

	if (new == NULL || alst == NULL)
		return ;
	if (!(*alst))
	{
		*alst = new;
		new->next = NULL;
		return ;
	}
	current = ft_lstlast(*alst);
	current->next = new;
	new->next = NULL;
}
