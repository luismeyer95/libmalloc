#include <libft_malloc.h>

extern void *global_start;

static inline void print_str(char *str)
{
	ft_putstr_fd(str, 1);
}

static inline void	print_base(uintptr_t nb, unsigned int base)
{
	static char basestr[17] = "0123456789abcdef";

	if (base > 16 || base < 2)
		return;
	if (nb < base)
		ft_putchar_fd(basestr[nb], 1);
	else
	{
		print_base(nb / base, base);
		ft_putchar_fd(basestr[nb % base], 1);
	}
}

static inline void foreach_node(t_node *lst, void (*f)(void *node))
{
	if (!f)
		return;
	while (lst)
	{
		f(lst);
		lst = lst->next;
	}
}

static inline void show_block(void *node)
{
	t_block *block = node;

	// if (!block->allocated)
	// 	return;

	print_str("0x");
	print_base((uintptr_t)SHIFT(block, SIZEOF_T_BLOCK), 16);
	print_str(" - 0x");
	print_base((uintptr_t)SHIFT(block, SIZEOF_T_BLOCK + block->size), 16);
	print_str(" : ");
	print_base(block->size, 10);
	print_str(" octets");
	if (block->allocated)
		print_str("\n");
	else
		print_str(" FREE\n");
}

static inline void show_heap(void *node)
{
	t_heap *heap = node;
	if (heap->group == TINY)
		print_str("TINY  : 0x");
	else if (heap->group == SMALL)
		print_str("SMALL : 0x");
	else
		print_str("LARGE : 0x");

	print_base((uintptr_t)heap, 16);
	print_str("\n");

	t_block *block_start = SHIFT(heap, SIZEOF_T_HEAP);
	foreach_node((void*)block_start, show_block);
}

void show_alloc_mem()
{
	foreach_node(global_start, show_heap);
}