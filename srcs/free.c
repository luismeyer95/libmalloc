#include <libft_malloc.h>

static inline bool is_valid_block(t_block *block, t_arena *arena)
{
	t_heap *heap_it = arena->heap_lst;
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

static inline t_heap *find_heap_of_block(t_block *block, t_arena *arena)
{
	if (!block || !arena || !arena->heap_lst)
		return NULL;
	while (block->prev)
		block = block->prev;
	return SHIFT(block, -SIZEOF_T_HEAP);
}

static inline bool can_del_heap(t_heap *heap, t_arena *arena)
{
	if (heap->group == LARGE)
		return true;

	t_block *block = (void*)SHIFT(heap, SIZEOF_T_HEAP);
	if (block->allocated || block->next)
		return false;

	t_heap *it = arena->heap_lst;
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

static inline void try_merge(t_block *block)
{
	if (can_merge(block->prev) && can_merge(block->next))
		merge_free_blocks(block->prev, block->next->next);
	else if (can_merge(block->prev))
		merge_free_blocks(block->prev, block->next);
	else if (can_merge(block->next))
		merge_free_blocks(block, block->next->next);
}

inline void free_impl(void *ptr, t_arena *arena)
{
	if (!ptr)
		return;
	t_block *block = SHIFT(ptr, -SIZEOF_T_BLOCK);
	if (!is_valid_block(block, arena))
	{
		ft_putstr_fd("free(): invalid pointer detected\n", 2);
		return;
	}
	block->allocated = false;
	try_merge(block);
	t_heap *heap = find_heap_of_block(block, arena);
	if (can_del_heap(heap, arena))
	{
		link_nodes((void*)heap->prev, (void*)heap->next);
		if (arena->heap_lst == (void*)heap)
			arena->heap_lst = heap->next;
		munmap(heap, SIZEOF_T_HEAP + heap->size);
	}
}

void free(void *ptr)
{
	try_init_state();

	if (!ptr)
		return;
	t_arena *arena = get_arena();
	if (!arena || arena->initialized == 0)
		return;

	lock_arena();
	free_impl(ptr, arena);
	unlock_arena(arena);

	t_ctl *ctl = malloc_ctl();
	if (ctl->dbg_flags.STACK_LOGGING)
		log_free_call(LOGFILE_PATH, ptr);
}