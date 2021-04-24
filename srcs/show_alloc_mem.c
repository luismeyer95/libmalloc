#include <libft_malloc.h>

static inline void foreach_node(t_node *lst, void (*f)(void *node))
{
	if (!f)
		return;
	while (lst)
	{
		f(lst);
		lst = lst->next;
	}
}

static inline void show_block(void *node)
{
	t_block *block = node;

	void *start = SHIFT(block, SIZEOF_T_BLOCK);
	void *end = SHIFT(block, SIZEOF_T_BLOCK + block->size);
	mprintf(1, "|%18p |%18p |%14u bytes |", start, end, block->size);
	if (block->allocated)
		mprintf(1, "\n");
	else
		mprintf(1, " (FREE)\n");
}

static inline void show_heap(void *node)
{
	static char *gpmap[] = {"TINY", "SMALL", "LARGE"};
	t_heap *heap = node;

	mprintf(1, "%-5s : %p\n", gpmap[(int)heap->group], heap);
	mprintf(1, "|%18s | %17s | %19s |\n", "start", "end", "size");
	t_block *block_start = SHIFT(heap, SIZEOF_T_HEAP);
	foreach_node((void*)block_start, show_block);
	mprintf(1, "\n");
}

void show_alloc_mem_impl()
{
	char sep[] = "=================================="
	"=============================\n";
	for (int i = 0; i < ARENA_COUNT; ++i)
	{
		if (arenas[i].initialized)
		{
			mprintf(1, "ARENA #%d\n", i);
			mprintf(1, sep);
			foreach_node(arenas[i].heap_lst, show_heap);
			mprintf(1, sep);
		}
	}
}

void show_alloc_mem()
{
	try_init_state();
	
	pthread_mutex_lock(&malloc_mtx);
	foreach_arena_mutex(pthread_mutex_trylock);
	show_alloc_mem_impl();
	foreach_arena_mutex(pthread_mutex_unlock);
	pthread_mutex_unlock(&malloc_mtx);
}

void hexdump_block(t_block *block)
{
	uint8_t *start = SHIFT(block, SIZEOF_T_BLOCK);
	uint8_t *end = SHIFT(start, block->size);

	for (uint8_t *ptr = start; ptr != end; ptr += 16)
	{
		mprintf(1, "%18p  ", ptr);
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
				mprintf(1, BLK WHTB "." RESET);
		}
		ft_putstr_fd("|\n", 1);
	}

}

void show_alloc_mem_ex(void *ptr)
{
	try_init_state();

	if (!ptr)
	{
		pthread_mutex_lock(&malloc_mtx);
		foreach_arena_mutex(pthread_mutex_trylock);
		show_alloc_mem_impl();
		foreach_arena_mutex(pthread_mutex_unlock);
		pthread_mutex_unlock(&malloc_mtx);
	}
	else
	{
		t_arena *arena = lock_arena();
		t_block *block = SHIFT(ptr, -SIZEOF_T_BLOCK);
		if (is_valid_block(block, arena))
			hexdump_block(block);
		else
			mprintf(1, "Invalid block\n");
		unlock_arena(arena);
	}
	
}