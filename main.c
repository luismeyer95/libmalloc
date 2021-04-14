#include <libft_malloc.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("%d\n", getpagesize());
	printf("t_block size = %zu\n", SIZEOF_T_BLOCK);
	printf("t_heap size = %zu\n", SIZEOF_T_HEAP);
	printf("tiny heap total size = %zu\n", tiny_heap_size());
	printf("small heap total size = %zu\n", small_heap_size());

}