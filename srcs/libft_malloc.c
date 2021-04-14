#include <libft_malloc.h>

int align(int a)
{
	return ALIGN(a);
}

size_t tiny_heap_size()
{
	size_t total = SIZEOF_T_HEAP + 100 * (TINY_MAX_ALLOC_SIZE + SIZEOF_T_BLOCK);
	int pagesize = getpagesize();
	return (total + pagesize - 1) & ~(pagesize - 1);
}

size_t small_heap_size()
{
	size_t total = SIZEOF_T_HEAP + 100 * (SMALL_MAX_ALLOC_SIZE + SIZEOF_T_BLOCK);
	int pagesize = getpagesize();
	return (total + pagesize - 1) & ~(pagesize - 1);
}

// void free(void *ptr)
// {
// 	(void)ptr;
// 	return;
// }

// void *malloc(size_t size)
// {
// 	return (void*)size;
// }

// void *realloc(void *ptr, size_t size)
// {
// 	return (char*)ptr + size;
// }