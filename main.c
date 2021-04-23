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

void print_test_footer()
{
	write(1,
		"---------\n"
		"| E N D |\n"
		"---------\n\n"
	, 31);
}

void show_mem_wrap()
{
	ft_putstr_fd("------------------\n", 1);
	show_alloc_mem();
	ft_putstr_fd("------------------\n", 1);
}

void random_ops(size_t number, size_t alloc_size_min, size_t alloc_size_max)
{
	// print_test_header("random_ops()");

	void *buf[4];
	ft_bzero(buf, 4 * sizeof(void*));
	void **ptr = NULL;
	for (size_t i = 0; i < number; ++i)
	{
		size_t index_roll = rand() % 4;
		ptr = &buf[index_roll];
		// printf(" ~ INDEX = %zu ~\n", index_roll);
		size_t size = alloc_size_min + (rand() % (alloc_size_max-alloc_size_min));

		int roll = rand() % 3;

		if (roll == 0 && *ptr)
		{
			// printf(" ~ FREE(%p) ~\n", *ptr);
			free(*ptr);
			*ptr = 0;
		}
		else if (roll == 1)
		{
			void *old = *ptr;
			*ptr = realloc(old, size);
			// printf(" ~ REALLOC(%p, %zu) = %p ~\n", old, size, *ptr);
			// printf(" ~ align(%zu) = %zu ~\n", size, align(size));
		}
		else if (!*ptr)
		{
			*ptr = malloc(size);
			// printf(" ~ MALLOC(%zu) = %p ~\n", size, *ptr);
			// printf(" ~ align(%zu) = %zu ~\n", size, align(size));
		}
		// show_mem_wrap();
		show_alloc_mem();
	}
	for (int i = 0; i < 4; ++i)
		free(buf[i]);

	print_test_footer();
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
	printf(" ~ FREE() index 1 ~\n");
	free(p[3]);
	show_mem_wrap();

	print_test_footer();
}

void test_realloc()
{
	print_test_header("test_realloc()");

	char *str = malloc(16);
	strcpy(str, "Hello world!");
	show_alloc_mem();
	str = realloc(str, 5);
	show_alloc_mem();
	str = realloc(str, 17);
	show_alloc_mem();
	free(str);
	show_alloc_mem();

	print_test_footer();
}

void test_fat_malloc()
{
	print_test_header("fat_malloc");
	size_t size = (size_t)-1;
	printf("~ MALLOC(%zu) ~\n", size);
	void *ptr = malloc(size);
	assert(!ptr);
	assert(errno == ENOMEM);
	print_test_footer();
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

	print_test_footer();
}

void *test_pthreads_func(void *bytes)
{
	random_ops(
		*(size_t*)bytes,
		*(size_t*)SHIFT(bytes, 8),
		*(size_t*)SHIFT(bytes, 16)
	);
	return NULL;
}

void	test_pthreads(size_t nb_threads, size_t ops_each,
		size_t alloc_size_min, size_t alloc_size_max)
{
	print_test_header("pthreads");

	// int stdout_backup, new;
	// fflush(stdout);
	// stdout_backup = dup(1);
	// new = open("/dev/null", O_WRONLY);
	// dup2(new, 1);
	// close(new);

	uint8_t bytes[32];
	memcpy(bytes, &ops_each, 8);
	memcpy(bytes+8, &alloc_size_min, 8);
	memcpy(bytes+16, &alloc_size_max, 8);


	pthread_t *ths = alloca(nb_threads * sizeof(pthread_t));
	for (size_t i = 0; i < nb_threads; ++i)
		pthread_create(&ths[i], NULL, test_pthreads_func, (void*)bytes);
	for (size_t i = 0; i < nb_threads; ++i)
		pthread_join(ths[i], NULL);
	
	// fflush(stdout);
	// dup2(stdout_backup, 1);
	// close(stdout_backup);

	print_test_footer();

}

int main()
{
	srand(time(NULL));
	// setvbuf(stdout, NULL, _IONBF, 0);
	
	// test_macros();
	// test_realloc();
	// test_fat_malloc();
	// test_coalesce();

	random_ops(3000, 0, 6000);
	// test_pthreads(600, 100, 0, 4096 * 128);

	// test_pthreads(100, 20, 0, 4096 * 128);
	// test_pthreads(1, 1, 0, 10000);

	show_alloc_mem();

}