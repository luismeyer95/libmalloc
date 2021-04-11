/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstmap_bonus.c                                .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: lumeyer <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/14 15:59:58 by lumeyer      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/19 17:07:58 by lumeyer     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	t_list	*new;
	t_list	*temp;
	t_list	*debnew;

	if ((!lst) || !f || !del || !(debnew = ft_lstnew(lst->content)))
		return (NULL);
	debnew->content = f(debnew->content);
	lst = lst->next;
	temp = debnew;
	while (lst)
	{
		if (!(new = ft_lstnew(lst->content)))
		{
			ft_lstclear(&debnew, del);
			return (NULL);
		}
		new->content = f(new->content);
		temp->next = new;
		temp = temp->next;
		lst = lst->next;
	}
	return (debnew);
}
