#include <libft_malloc.h>



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

void show_alloc_mem_impl()
{
	for (int i = 0; i < ARENA_COUNT; ++i)
	{
		if (arenas[i].initialized)
		{
			print_str("ARENA #");
			print_base((uintptr_t)i, 10);
			print_str("\n");
			print_str("==========================\n");
			foreach_node(arenas[i].heap_lst, show_heap);
			print_str("==========================\n");
		}
	}
}

void show_alloc_mem()
{
	pthread_mutex_lock(&malloc_mtx);
	foreach_arena_mutex(pthread_mutex_trylock);
	show_alloc_mem_impl();
	foreach_arena_mutex(pthread_mutex_unlock);
	pthread_mutex_unlock(&malloc_mtx);
}