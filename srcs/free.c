#include <libft_malloc.h>

static inline bool is_valid_block(t_block *block)
{
	t_heap *heap_it = global_start;
	while (heap_it)
	{
		t_block *block_it = SHIFT(heap_it, SIZEOF_T_HEAP);
		while (block_it)
		{
			if (block_it == block)
				return true;
			block_it = block_it->next;
		}
		heap_it = heap_it->next;
	}
	return false;
}

static inline t_heap *find_heap_of_block(t_block *block)
{
	if (!global_start)
		return NULL;
	while (block->prev)
		block = block->prev;
	return SHIFT(block, -SIZEOF_T_HEAP);
}

static inline bool can_del_heap(t_heap *heap)
{
	t_block *block = (void*)SHIFT(heap, SIZEOF_T_HEAP);
	if (block->allocated || block->next)
		return false;

	if (heap->group == LARGE)
		return true;
	t_heap *it = global_start;
	size_t count = 0;
	while (it)
	{
		if (it->group == heap->group)
			++count;
		if (count == 2)
			return true;
		it = it->next;
	}
	return false;
}

static inline bool can_merge(t_block *block)
{
	return (block && !block->allocated);
}

static inline void merge_free_blocks(t_block *beg, t_block *end)
{
	t_block *it = beg;
	size_t merged_size = 0;
	while (it != end)
	{
		merged_size += it->size + SIZEOF_T_BLOCK;
		it = it->next;
	}
	beg->size = merged_size - SIZEOF_T_BLOCK;
	link_nodes((void*)beg, (void*)end);
}

inline void free_impl(void *ptr)
{
	if (!ptr)
		return;
	t_block *block = SHIFT(ptr, -SIZEOF_T_BLOCK);
	if (!is_valid_block(block))
	{
		write(1, "free(): tf you passing a random pointer bitch?\n", 48);
		return;
	}
	block->allocated = false;	
	t_heap *heap = find_heap_of_block(block);
	if (can_merge(block->prev) && can_merge(block->next))
		merge_free_blocks(block->prev, block->next->next);
	else if (can_merge(block->prev))
		merge_free_blocks(block->prev, block->next);
	else if (can_merge(block->next))
		merge_free_blocks(block, block->next->next);
	if (can_del_heap(heap))
	{
		link_nodes((void*)heap->prev, (void*)heap->next);
		if (global_start == (void*)heap)
			global_start = heap->next;
		munmap(heap, SIZEOF_T_HEAP + heap->size);
	}
}

void free(void *ptr)
{
	pthread_mutex_lock(&malloc_mtx);

	static t_debug_flags flags;
	if (!global_start)
		fetch_debug_flags(&flags);

	ft_putstr_fd("free 0x", 1);
	print_base((uintptr_t)ptr, 16);
	ft_putstr_fd("\n", 1);
	
	free_impl(ptr);
	// show_alloc_mem_impl();

	pthread_mutex_unlock(&malloc_mtx);
}