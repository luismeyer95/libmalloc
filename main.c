// #include <libft_malloc.h>
#include <libft_malloc_internals.h>
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
	mprintf(1, "\n| TEST %s |\n", str);
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

void test_random_ops(size_t number, size_t alloc_size_min, size_t alloc_size_max)
{
	// print_test_header("test_random_ops()");

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
		else if (roll == 1 && *ptr)
		{
			void *old = *ptr;
			void *new = realloc(old, size);
			if (new)
				*ptr = new;
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
		// show_alloc_mem();
	}
	for (int i = 0; i < 4; ++i)
		free(buf[i]);

	// print_test_footer();
}

void test_coalesce()
{
	print_test_header("test_coalesce()");
	void *p[4];
	mprintf(1, " ~ MALLOC() 4 x 32 bytes ~\n");
	p[0] = malloc(32);
	p[1] = malloc(32);
	p[2] = malloc(32);
	p[3] = malloc(32);
	show_mem_wrap();

	mprintf(1, " ~ FREE() index 0 and 2 ~\n");
	free(p[0]);
	free(p[2]);
	show_mem_wrap();

	mprintf(1, " ~ FREE() index 1 ~\n");
	free(p[1]);
	show_mem_wrap();
	mprintf(1, " ~ FREE() index 1 ~\n");
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
	mprintf(1, "~ MALLOC(%u) ~\n", size);
	void *ptr = malloc(size);
	assert(!ptr);
	assert(errno == ENOMEM);
	print_test_footer();
}

void test_macros()
{
	print_test_header("test_macros()");

	mprintf(1, "pagesize = %d\n", getpagesize());
	mprintf(1, "alignment = %d\n", ALIGNMENT);
	mprintf(1, "t_block size = %zu\n", SIZEOF_T_BLOCK);
	mprintf(1, "t_heap size = %zu\n", SIZEOF_T_HEAP);
	mprintf(1, "tiny heap total size = %zu\n", TINY_HEAP_SIZE);
	mprintf(1, "small heap total size = %zu\n", SMALL_HEAP_SIZE);

	print_test_footer();
}

void *test_pthreads_func(void *bytes)
{
	test_random_ops(
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

	uint8_t bytes[32];
	memcpy(bytes, &ops_each, 8);
	memcpy(bytes+8, &alloc_size_min, 8);
	memcpy(bytes+16, &alloc_size_max, 8);

	pthread_t *ths = alloca(nb_threads * sizeof(pthread_t));
	for (size_t i = 0; i < nb_threads; ++i)
		pthread_create(&ths[i], NULL, test_pthreads_func, (void*)bytes);
	for (size_t i = 0; i < nb_threads; ++i)
		pthread_join(ths[i], NULL);
	
	print_test_footer();

}

void test_scribble()
{
	print_test_header("scribble");
	setenv("MallocScribble", "1", 1);

	void *ptr = malloc(64);
	ft_memset(ptr, 1, 64);
	free(ptr);
	
	int is_scribbled = 1;
	for (int i = 0; i < 64; ++i)
		if ( ((char*)ptr)[i] != 0x55)
			is_scribbled = 0;

	assert(is_scribbled);

	unsetenv("MallocScribble");
	print_test_footer();

}

void test_prescribble()
{
	print_test_header("prescribble");
	setenv("MallocPreScribble", "1", 1);

	void *ptr = malloc(64);

	int is_scribbled = 1;
	for (int i = 0; i < 64; ++i)
		if (((uint8_t*)ptr)[i] != 0xAA)
			is_scribbled = 0;

	assert(is_scribbled);
	free(ptr);

	unsetenv("MallocPreScribble");
	print_test_footer();
}

void test_show_alloc_mem_ex()
{
	print_test_header("show_alloc_mem_ex");

	void *ptr = malloc(32);
	strcpy(ptr, "Hello world!");
	show_alloc_mem_ex();
	free(ptr);

	print_test_footer();
}

void test_limits()
{
	print_test_header("limits");

	char *t;
    struct rlimit rpl;
	getrlimit(RLIMIT_AS, &rpl);
	rlim_t bkp = rpl.rlim_cur;
    rpl.rlim_cur = 1000;
    if (setrlimit(RLIMIT_AS, &rpl) < 0)
		perror("setrlimit()");

	t = malloc(1);
	assert(!t);

	rpl.rlim_cur = bkp;
	 if (setrlimit(RLIMIT_AS, &rpl) < 0)
		perror("setrlimit()");
	print_test_footer();
}

int main()
{
	srand(time(NULL));
	
	test_macros();
	test_realloc();
	test_fat_malloc();
	test_coalesce();
	test_show_alloc_mem_ex();
	// test_scribble();
	// test_prescribble();

	// test_random_ops(3000, 5000, 100000);
	test_pthreads(3000, 200, 0, 4096 * 128);

	show_alloc_mem();

}