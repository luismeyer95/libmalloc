#include <libft_malloc.h>

extern void *global_start;

void *ft_realloc(void *ptr, size_t size)
{
	if (!ptr)
		return ft_malloc(size);
	else if (!size)
	{
		ft_free(ptr);
		return NULL;
	}	
	t_block *block = SHIFT(ptr, -SIZEOF_T_BLOCK);
	if (!block->allocated)
		return NULL;
	if (block->size == ALIGN(size))
		return ptr;
	uint8_t *new_malloc = ft_malloc(size);
	if (!new_malloc)
		return NULL;
	size_t lencopy = block->size < size ? block->size : size;
	ft_memcpy(new_malloc, ptr, lencopy);
	ft_free(ptr);
	return new_malloc;
}