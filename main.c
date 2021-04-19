#include <libft_malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <execinfo.h>
#include <alloca.h>

void print_test_header(char *str)
{
	char buf[512];
	size_t framelen = strlen("TEST ") + strlen(str) + 4;
	assert(framelen < 512);
	memset(buf, '-', framelen);
	write(1, buf, framelen);
	printf("\n| TEST %s |\n", str);
	write(1, buf, framelen);
	write(1, "\n", 1);
}

void show_mem_wrap()
{
	ft_putstr_fd("------------------\n", 1);
	show_alloc_mem();
	ft_putstr_fd("------------------\n", 1);
}

void random_ops(int number, size_t alloc_size_min, size_t alloc_size_max)
{
	// print_test_header("random_ops()");

	void *buf[4];
	ft_bzero(buf, 4 * sizeof(void*));
	void **ptr = NULL;
	for (int i = 0; i < number; ++i)
	{
		size_t index_roll = rand() % 4;
		ptr = &buf[index_roll];
		// printf(" ~ INDEX = %zu ~\n", index_roll);
		size_t size = alloc_size_min + (rand() % (alloc_size_max-alloc_size_min));

		int roll = rand() % 3;

		// else if (roll == 1)
		// {
		// 	void *old = *ptr;
		// 	*ptr = realloc(old, size);
		// 	// printf(" ~ REALLOC(%p, %zu) = %p ~\n", old, size, *ptr);
		// 	// printf(" ~ align(%zu) = %zu ~\n", size, align(size));

		// }
		if (roll == 0 && *ptr)
		{
			// printf(" ~ FREE(%p) ~\n", *ptr);
			free(*ptr);
			*ptr = 0;
		}
		else if (!*ptr)
		{
			*ptr = malloc(size);
			// printf(" ~ MALLOC(%zu) = %p ~\n", size, *ptr);
			// printf(" ~ align(%zu) = %zu ~\n", size, align(size));
		}
		show_mem_wrap();
	}
	for (int i = 0; i < 4; ++i)
		free(buf[i]);
}

void test_coalesce()
{
	print_test_header("test_coalesce()");
	void *p[4];
	printf(" ~ MALLOC() 4 x 32 bytes ~\n");
	p[0] = malloc(32);
	p[1] = malloc(32);
	p[2] = malloc(32);
	p[3] = malloc(32);
	show_mem_wrap();

	printf(" ~ FREE() index 0 and 2 ~\n");
	free(p[0]);
	free(p[2]);
	show_mem_wrap();

	printf(" ~ FREE() index 1 ~\n");
	free(p[1]);
	show_mem_wrap();
}

void test_realloc()
{
	// print_test_header("test_realloc()");

	char *str = malloc(16);
	strcpy(str, "Hello world!");
	show_alloc_mem();
	str = realloc(str, 5);
	show_alloc_mem();
	str = realloc(str, 17);
	show_alloc_mem();
	free(str);
}

void test_fat_malloc()
{
	// print_test_header("fat_malloc");
	size_t size = (size_t)-1;
	// printf("~ MALLOC(%zu) ~\n", size);
	void *ptr = malloc(size);
	assert(!ptr);
	assert(errno == ENOMEM);
}

void test_macros()
{
	print_test_header("test_macros()");

	printf("pagesize = %d\n", getpagesize());
	printf("alignment = %d\n", ALIGNMENT);
	printf("t_block size = %zu\n", SIZEOF_T_BLOCK);
	printf("t_heap size = %zu\n", SIZEOF_T_HEAP);
	printf("tiny heap total size = %zu\n", TINY_HEAP_SIZE);
	printf("small heap total size = %zu\n", SMALL_HEAP_SIZE);
}

void *test_pthreads_func(void *ops)
{
	random_ops((size_t)ops, 0, 256);
	return NULL;
}

void test_pthreads(size_t ops_each, size_t nb_threads)
{
	// print_test_header("pthreads");

	// int stdout_backup, new;
	// fflush(stdout);
	// stdout_backup = dup(1);
	// new = open("/dev/null", O_WRONLY);
	// dup2(new, 1);
	// close(new);

	pthread_t *ths = alloca(nb_threads * sizeof(pthread_t));
	for (size_t i = 0; i < nb_threads; ++i)
		pthread_create(&ths[i], NULL, test_pthreads_func, (void*)(uintptr_t)(ops_each));
	for (size_t i = 0; i < nb_threads; ++i)
		pthread_join(ths[i], NULL);
	
	// fflush(stdout);
	// dup2(stdout_backup, 1);
	// close(stdout_backup);

	show_alloc_mem();
}


int main()
{
	srand(time(NULL));
	
	// test_macros();
	// test_realloc();
	// test_coalesce();
	// random_ops(3000, 0, 6000);
	// free(malloc(3));

	test_pthreads(50, 50);


	// test_fat_malloc();
	show_alloc_mem();

	
}