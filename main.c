#include <libft_malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <time.h>

void random_ops(int number)
{
	void *ptr = NULL;
	for (int i = 0; i < number; ++i)
	{
		size_t size = 16 * (rand() % 48);
		int roll = rand() % 3;

		if (roll == 0 && ptr)
		{
			printf(" ~ FREE(%p) ~\n", ptr);
			ft_free(ptr);
			ptr = 0;
		}
		else if (roll == 1)
		{
			printf(" ~ REALLOC(%p, %zu) ~\n", ptr, size);
			ptr = ft_realloc(ptr, size);
		}
		else
		{
			printf(" ~ MALLOC(%zu) ~\n", size);
			ptr = ft_malloc(size);
		}
		ft_putstr_fd("------------------\n", 1);
		show_alloc_mem();
		ft_putstr_fd("------------------\n", 1);
	}
}

void assert(int cond, char *err)
{
	if (!cond)
	{
		dprintf(2, "Failed assertion: %s\n", err);
		exit(1);
	}
}

void test_realloc()
{
	char *str = ft_malloc(16);
	strcpy(str, "Hello world!");
	show_alloc_mem();
	str = ft_realloc(str, 5);
	show_alloc_mem();
}

int main()
{
	srand(time(NULL));
	// printf("%d\n", getpagesize());
	// printf("t_block size = %zu\n", SIZEOF_T_BLOCK);
	// printf("t_heap size = %zu\n", SIZEOF_T_HEAP);
	// printf("tiny heap total size = %zu\n", tiny_heap_size());
	// printf("small heap total size = %zu\n", small_heap_size());

	// test_realloc();
	random_ops(1000);

	
}