#include <libft_malloc.h>

void free(void *ptr)
{
	(void)ptr;
	return;
}

void *malloc(size_t size)
{
	return (void*)size;
}

void *realloc(void *ptr, size_t size)
{
	return (char*)ptr + size;
}