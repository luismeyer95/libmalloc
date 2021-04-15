#include <libft_malloc.h>

void *global_start = NULL;

static inline size_t align_on(size_t nb, size_t alignment)
{
	return (nb + alignment - 1) & ~(alignment - 1); 
}

static inline void *create_heap(t_group heap_group, size_t heap_size)
{
	void *map = NULL;
	map = mmap(NULL, heap_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (map == MAP_FAILED)
		return NULL;
	t_heap *heap = map;
	heap->group = heap_group;
	heap->size = heap_size - SIZEOF_T_HEAP;
	heap->prev = NULL;
	heap->next = NULL;
	t_block *whole_block = SHIFT(heap, SIZEOF_T_HEAP);
	whole_block->allocated = false;
	whole_block->size = heap_size - SIZEOF_T_BLOCK - SIZEOF_T_HEAP;
	whole_block->prev = NULL;
	whole_block->next = NULL;
	return map;
}

static inline t_group get_alloc_heap_group(size_t aligned_alloc_size)
{
	if (aligned_alloc_size > SMALL_MAX_ALLOC_SIZE)
		return LARGE;
	else if (aligned_alloc_size > TINY_MAX_ALLOC_SIZE)
		return SMALL;
	else
		return TINY;
}

static inline void *create_heap_from_alloc_size(size_t alloc_size)
{
	void *heap = NULL;
	size_t aligned_alloc_size = ALIGN(alloc_size);
	t_group heap_group = get_alloc_heap_group(aligned_alloc_size);
	if (heap_group == TINY)
		heap = create_heap(TINY, TINY_HEAP_SIZE);
	else if (heap_group == SMALL)
		heap = create_heap(SMALL, SMALL_HEAP_SIZE);
	else
	{
		size_t new_heap_size = align_on(alloc_size, getpagesize());
		heap = create_heap(LARGE, new_heap_size);
	}
	return heap;
}

inline void link_nodes(t_node *n1, t_node *n2)
{
	if (n1)
		n1->next = n2;
	if (n2)
		n2->prev = n1;
}

inline void insert_after_node(t_node *node, t_node *new)
{
	t_node *next_node = node->next;
	link_nodes(node, new);
	link_nodes(new, next_node);
}

static inline void *allocate_and_split(t_block *block, size_t aligned_alloc_size)
{
	size_t remaining_space = block->size - aligned_alloc_size;
	if (remaining_space > SIZEOF_T_BLOCK)
	{
		t_block *free_block = SHIFT(block, SIZEOF_T_BLOCK + aligned_alloc_size);
		free_block->allocated = false;
		free_block->size = remaining_space - SIZEOF_T_BLOCK;
		insert_after_node((void*)block, (void*)free_block);
	}
	block->allocated = true;
	block->size = aligned_alloc_size;
	return (SHIFT(block, SIZEOF_T_BLOCK));
}

static inline void *find_fit(size_t alloc_size, t_heap *heap)
{
	size_t aligned_alloc_size = ALIGN(alloc_size);
	t_block *block = SHIFT(heap, SIZEOF_T_HEAP);
	while (block)
	{
		if (block->size >= aligned_alloc_size && block->allocated == false)
			return allocate_and_split(block, aligned_alloc_size);
		block = block->next;
	}
	return (NULL);
}

static inline void insert_sort_heap(t_heap *new_heap)
{
	if (!global_start)
		global_start = new_heap;
	else
	{
		if (new_heap < (t_heap*)global_start)
		{
			void *old_start = global_start;
			global_start = new_heap;
			return link_nodes(global_start, old_start);
		}
		t_heap *heap_it = global_start;
		while (heap_it->next)
		{
			if (new_heap < heap_it->next)
				return insert_after_node((void*)heap_it, (void*)new_heap);
			heap_it = heap_it->next;
		}
		return insert_after_node((void*)heap_it, (void*)new_heap);
	}
}

static inline void *search_available_heaps(size_t alloc_size, t_group alloc_heap_group)
{
	t_heap *heap = global_start;
	while (heap)
	{
		if (heap->group == alloc_heap_group)
		{
			void *alloc = find_fit(alloc_size, heap);
			if (alloc)
				return alloc;
		}
		heap = heap->next;
	}
	return NULL;
}

void *ft_malloc(size_t size)
{
	if (!size)
		return NULL;
	t_group alloc_heap_group = get_alloc_heap_group(size);
	void *alloc = search_available_heaps(size, alloc_heap_group);
	if (alloc)
		return alloc;
	t_heap *new_heap = create_heap_from_alloc_size(size);
	if (!new_heap)
	{
		errno = ENOMEM;
		return NULL;
	}
	insert_sort_heap(new_heap);
	return find_fit(size, new_heap);
}
