#include <libft_malloc_internals.h>

void hexdump_block(t_block *block)
{
	uint8_t *start = SHIFT(block, SIZEOF_T_BLOCK);
	uint8_t *end = SHIFT(start, block->size);

	for (uint8_t *ptr = start; ptr != end; ptr += 16)
	{
		mprintf(1, "\t%18p  ", ptr);
		for (int i = 0; i < 8; ++i)
			mprintf(1, "%2x ", ptr[i]);
		mprintf(1, " ");
		for (int i = 8; i < 16; ++i)
			mprintf(1, "%2x ", ptr[i]);
		mprintf(1, " |");
		for (int i = 0; i < 16; ++i)
		{
			if (ft_isprint(ptr[i]))
				ft_putchar_fd(ptr[i], 1);
			else
				ft_putchar_fd('.', 1);
		}
		ft_putstr_fd("|\n", 1);
	}
	ft_putchar_fd('\n', 1);
}

static inline void show_block_ex(void *node)
{
	t_block *block = node;

	void *start = SHIFT(block, SIZEOF_T_BLOCK);
	void *end = SHIFT(block, SIZEOF_T_BLOCK + block->size);
	mprintf(1, "\t>> BLOCK %p - %p (%u bytes)", start, end, block->size);
	if (block->allocated)
	{
		mprintf(1, "\n");
		hexdump_block(block);
	}
	else
		mprintf(1, " (FREE)\n");

}

static inline void show_heap_ex(void *node)
{
	static char *gpmap[] = {"TINY", "SMALL", "LARGE"};
	t_heap *heap = node;

	mprintf(1, "%-5s : %p\n\n", gpmap[(int)heap->group], heap);
	t_block *block_start = SHIFT(heap, SIZEOF_T_HEAP);
	foreach_node((void*)block_start, show_block_ex);
	mprintf(1, "\n");
}

void show_alloc_mem_ex_impl()
{
	char sep[] = "=================================="
	"==============================================================\n";
	for (int i = 0; i < ARENA_COUNT; ++i)
	{
		if (arenas[i].initialized)
		{
			mprintf(1, "ARENA #%d\n", i);
			mprintf(1, sep);
			foreach_node(arenas[i].heap_lst, show_heap_ex);
			mprintf(1, sep);
		}
	}
}

void show_alloc_mem_ex()
{
	try_init_state();
	pthread_mutex_lock(&malloc_mtx);
	foreach_arena_mutex(pthread_mutex_trylock);
	show_alloc_mem_ex_impl();
	foreach_arena_mutex(pthread_mutex_unlock);
	pthread_mutex_unlock(&malloc_mtx);
}
