#include <libft_malloc.h>

void *realloc_impl(void *ptr, size_t size, t_arena *arena)
{
	if (!ptr)
		return malloc_impl(size, arena);
	else if (!size)
	{
		free_impl(ptr, arena);
		return NULL;
	}
	t_block *block = SHIFT(ptr, -SIZEOF_T_BLOCK);
	if (!block->allocated)
		return NULL;
	if (block->size == align(size))
		return ptr;
	uint8_t *new_malloc = malloc_impl(size, arena);
	if (!new_malloc)
		return NULL;
	size_t lencopy = block->size < size ? block->size : size;
	ft_memcpy(new_malloc, ptr, lencopy);
	free_impl(ptr, arena);
	return new_malloc;
}

void *realloc(void *ptr, size_t size)
{
	try_init_state();
	t_arena *locked_arena = lock_arena();
	void *alloc = realloc_impl(ptr, size, locked_arena);
	pthread_mutex_unlock(&locked_arena->arena_mtx);
	return alloc;
}