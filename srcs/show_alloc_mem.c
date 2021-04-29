#include <libft_malloc_internals.h>

inline void foreach_node(t_node *lst, void (*f)(void *node))
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

	size_t total_allocated = 0;
	t_block *b = SHIFT(heap, SIZEOF_T_HEAP);
	while (b)
	{
		total_allocated += b->allocated * b->size;
		b = b->next;
	}

	mprintf(1, "%-5s : %p - %u allocated bytes\n",
	gpmap[(int)heap->group], heap, total_allocated);
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
