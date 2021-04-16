#include <libft_malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <execinfo.h>

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
	print_test_header("random_ops()");

	void *buf[4];
	ft_bzero(buf, 4 * sizeof(void*));
	void **ptr = NULL;
	for (int i = 0; i < number; ++i)
	{
		size_t index_roll = rand() % 4;
		ptr = &buf[index_roll];
		printf(" ~ INDEX = %zu ~\n", index_roll);
		size_t size = alloc_size_min + (rand() % (alloc_size_max-alloc_size_min));

		int roll = rand() % 3;

		if (roll == 0 && *ptr)
		{
			printf(" ~ FREE(%p) ~\n", *ptr);
			ft_free(*ptr);
			*ptr = 0;
		}
		else if (roll == 1)
		{
			void *old = *ptr;
			*ptr = ft_realloc(old, size);
			printf(" ~ REALLOC(%p, %zu) = %p ~\n", old, size, *ptr);
			printf(" ~ ALIGN(%zu) = %zu ~\n", size, ALIGN(size));

		}
		else
		{
			*ptr = ft_malloc(size);
			printf(" ~ MALLOC(%zu) = %p ~\n", size, *ptr);
			printf(" ~ ALIGN(%zu) = %zu ~\n", size, ALIGN(size));
		}
		show_mem_wrap();
	}
}

void test_coalesce()
{
	print_test_header("test_coalesce()");
	void *p[4];
	printf(" ~ MALLOC() 4 x 32 bytes ~\n");
	p[0] = ft_malloc(32);
	p[1] = ft_malloc(32);
	p[2] = ft_malloc(32);
	p[3] = ft_malloc(32);
	show_mem_wrap();

	printf(" ~ FREE() index 0 and 2 ~\n");
	ft_free(p[0]);
	ft_free(p[2]);
	show_mem_wrap();

	printf(" ~ FREE() index 1 ~\n");
	ft_free(p[1]);
	show_mem_wrap();
}

void test_realloc()
{
	print_test_header("test_realloc()");

	char *str = ft_malloc(16);
	strcpy(str, "Hello world!");
	show_alloc_mem();
	str = ft_realloc(str, 5);
	show_alloc_mem();
	str = ft_realloc(str, 17);
	show_alloc_mem();
	ft_free(str);
}

void test_fat_malloc()
{
	print_test_header("fat_malloc");
	size_t size = (size_t)-1;
	printf("~ MALLOC(%zu) ~\n", size);
	void *ptr = ft_malloc(size);
	assert(!ptr);
	assert(errno == ENOMEM);
}

void test_macros()
{
	print_test_header("test_macros()");

	printf("%d\n", getpagesize());
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

void test_pthreads(size_t ops_each)
{
	print_test_header("pthreads");

	int bak, new;
	fflush(stdout);
	bak = dup(1);
	new = open("/dev/null", O_WRONLY);
	dup2(new, 1);
	close(new);

	pthread_t ths[32];
	for (int i = 0; i < 32; ++i)
		pthread_create(&ths[i], NULL, test_pthreads_func, (void*)(uintptr_t)(ops_each));
	for (int i = 0; i < 32; ++i)
		pthread_join(ths[i], NULL);
	
	fflush(stdout);
	dup2(bak, 1);
	close(bak);
}


int main()
{
	srand(time(NULL));
	
	// test_macros();
	// test_realloc();
	// test_coalesce();

	test_pthreads(100);

	// test_fat_malloc();
	// random_ops(600, 0, 128);
	
}