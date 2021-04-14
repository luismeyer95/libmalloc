#include <libft_malloc.h>

size_t align_on(size_t nb, size_t alignment)
{
	return (nb + alignment - 1) & ~(alignment - 1); 
}

size_t tiny_heap_size()
{
	size_t total = SIZEOF_T_HEAP + 100 * (TINY_MAX_ALLOC_SIZE + SIZEOF_T_BLOCK);
	return align_on(total, getpagesize());
}

size_t small_heap_size()
{
	size_t total = SIZEOF_T_HEAP + 100 * (SMALL_MAX_ALLOC_SIZE + SIZEOF_T_BLOCK);
	return align_on(total, getpagesize());
}

void *global_start = NULL;

void *create_heap(size_t heap_size)
{
	void *map = NULL;
	map = mmap(NULL, heap_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (map == MAP_FAILED)
		return NULL;
	t_heap *heap = map;
	if (heap_size > small_heap_size())
		heap->group = LARGE;
	else if (heap_size > tiny_heap_size())
		heap->group = SMALL;
	else
		heap->group = TINY;
	heap->next = NULL;
	heap->prev = NULL;
	heap->size = heap_size - SIZEOF_T_HEAP;
	t_block *whole_block = SHIFT(heap, SIZEOF_T_HEAP);
	whole_block->allocated = false;
	whole_block->size = heap_size - SIZEOF_T_BLOCK - SIZEOF_T_HEAP;
	whole_block->prev = NULL;
	whole_block->next = NULL;
	return map;
}

t_group get_alloc_heap_group(size_t aligned_alloc_size)
{
	if (aligned_alloc_size > SMALL_MAX_ALLOC_SIZE)
		return LARGE;
	else if (aligned_alloc_size > TINY_MAX_ALLOC_SIZE)
		return SMALL;
	else
		return TINY;
}

void *create_heap_from_alloc_size(size_t alloc_size)
{
	void *heap = NULL;
	size_t aligned_alloc_size = ALIGN(alloc_size);
	t_group heap_group = get_alloc_heap_group(aligned_alloc_size);
	if (heap_group == TINY)
		heap = create_heap(tiny_heap_size());
	else if (heap_group == SMALL)
		heap = create_heap(small_heap_size());
	else
	{
		size_t new_heap_size = align_on(alloc_size, getpagesize());
		heap = create_heap(new_heap_size);
	}
	return heap;
}

void link_nodes(t_node *n1, t_node *n2)
{
	if (n1)
		n1->next = n2;
	if (n2)
		n2->prev = n1;
}

void insert_after_node(t_node *node, t_node *new)
{
	t_node *next_node = node->next;
	link_nodes(node, new);
	link_nodes(new, next_node);
}

void *allocate_and_split(t_block *block, size_t aligned_alloc_size)
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

void *find_fit(size_t alloc_size, t_heap *heap)
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

void insert_sort_heap(t_heap *new_heap)
{
	if (!global_start)
		global_start = new_heap;
	else
	{
		if (new_heap < global_start)
		{
			void *old_start = global_start;
			global_start = new_heap;
			link_nodes(global_start, old_start);
			return;
		}
		t_heap *heap_it = global_start;
		while (heap_it->next)
		{
			if (new_heap < heap_it->next)
			{
				insert_after_node(heap_it, new_heap);
				return;
			}
			heap_it = heap_it->next;
		}
		return insert_after_node(heap_it, new_heap);
	}
}

void *ft_malloc(size_t size)
{
	if (!size)
		return NULL;
	t_heap *heap = global_start;
	t_group alloc_heap_group = get_alloc_heap_group(size);
	while (heap)
	{
		if (heap->group == alloc_heap_group)
		{
			void *alloc = find_fit(size, heap);
			if (alloc)
				return alloc;
		}
		heap = heap->next;
	}
	t_heap *new_heap = create_heap_from_alloc_size(size);
	if (!new_heap)
		return NULL;
	// pushing new heaps at the front
	// void *old_start = global_start;
	// global_start = new_heap;
	// link_nodes(global_start, old_start);
	insert_sort_heap(new_heap);
	return find_fit(size, new_heap);
}

void node_foreach(t_node *lst, void (*f)(void *node))
{
	if (!lst || !f)
		return;
	while (lst)
	{
		f(lst);
		lst = lst->next;
	}
}

void show_block(void *node)
{
	
}

void show_heap(void *node)
{
	t_heap *heap = node;
	if (heap->group == TINY)
		ft_putstr_fd("TINY", 1);
	else if (heap->group == SMALL)
		ft_putstr_fd("SMALL", 1);
	else
		ft_putstr_fd("LARGE", 1);
	ft_putstr_fd(" : ", 1);
	t_block *block_start = SHIFT(heap, SIZEOF_T_HEAP);
	node_foreach(block_start, show_block);
}

void show_alloc_mem()
{
	node_foreach(global_start, show_heap);
}

void ft_free(void *ptr)
{
	(void)ptr;
	return;
}


void *ft_realloc(void *ptr, size_t size)
{
	return (char*)ptr + size;
}