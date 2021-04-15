#include <libft_malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <time.h>

int main()
{
	// printf("%d\n", getpagesize());
	// printf("t_block size = %zu\n", SIZEOF_T_BLOCK);
	// printf("t_heap size = %zu\n", SIZEOF_T_HEAP);
	// printf("tiny heap total size = %zu\n", tiny_heap_size());
	// printf("small heap total size = %zu\n", small_heap_size());


	srand(time(NULL));

	char *str;
	for (int i = 0; i < 100; ++i)
	{
		str = ft_malloc(20 + (rand() % 5000));
		show_alloc_mem();
		ft_putstr_fd("------------------\n", 1);
		ft_free(str);
	}

}