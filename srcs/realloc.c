#include <libft_malloc.h>

void *ft_realloc(void *ptr, size_t size)
{
	pthread_mutex_lock(&malloc_mtx);
	if (!ptr)
	{
		ptr = malloc_impl(size);
		pthread_mutex_unlock(&malloc_mtx);
		return ptr;
	}
	else if (!size)
	{
		free_impl(ptr);
		pthread_mutex_unlock(&malloc_mtx);
		return NULL;
	}
	t_block *block = SHIFT(ptr, -SIZEOF_T_BLOCK);
	if (!block->allocated)
	{
		pthread_mutex_unlock(&malloc_mtx);
		return NULL;
	}
	if (block->size == ALIGN(size))
	{
		pthread_mutex_unlock(&malloc_mtx);
		return ptr;
	}
	uint8_t *new_malloc = malloc_impl(size);
	if (!new_malloc)
		return NULL;
	size_t lencopy = block->size < size ? block->size : size;
	ft_memcpy(new_malloc, ptr, lencopy);
	free_impl(ptr);
	pthread_mutex_unlock(&malloc_mtx);
	return new_malloc;
}