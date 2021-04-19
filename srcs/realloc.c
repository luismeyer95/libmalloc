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

	LIBMALLOC_LOCK_WRAP(
		ft_putstr_fd("realloc 0x", 1);
		print_base((uintptr_t)ptr, 16);
		ft_putstr_fd(" = 0x", 1);
		print_base((uintptr_t)alloc, 16);
		ft_putstr_fd("\n", 1);
	);

	pthread_mutex_unlock(&locked_arena->arena_mtx);
	
	return alloc;

	// pthread_mutex_lock(&malloc_mtx);

	// void *alloc = realloc_impl(ptr, size);

	// ft_putstr_fd("realloc = 0x", 1);
	// print_base((uintptr_t)alloc, 16);
	// ft_putstr_fd("\n", 1);

	// pthread_mutex_unlock(&malloc_mtx);
	// return alloc;
}