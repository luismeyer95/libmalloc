#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <stdbool.h>
# include <libft.h>

// remove later
# include <string.h>
# include <errno.h>
# include <stdlib.h>

typedef	enum	s_group
{
	TINY, SMALL, LARGE
}				t_group;

typedef	struct		s_node
{
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

typedef	struct		s_heap
{
	struct s_heap	*prev;
	struct s_heap	*next;
	size_t			size;
	t_group			group;
}					t_heap;

typedef	struct		s_block
{
	struct s_block	*prev;
	struct s_block	*next;
	size_t			size;
	bool 			allocated;
}					t_block;

# ifndef ALIGNMENT
#  define ALIGNMENT 16
# endif

# define SIZEOF_T_HEAP align(sizeof(t_heap))
# define SIZEOF_T_BLOCK align(sizeof(t_block))

# define TINY_MAX_ALLOC_SIZE 128
# define SMALL_MAX_ALLOC_SIZE 4096

# define SHIFT(x, size) ((void*)((char*)(x) + (size)))
# define ABS(x) ((x) > 0 ? (x) : -(x))

# define TINY_HEAP_SIZE (align_on(SIZEOF_T_HEAP + \
	100 * (TINY_MAX_ALLOC_SIZE + SIZEOF_T_BLOCK), getpagesize()))

# define SMALL_HEAP_SIZE (align_on(SIZEOF_T_HEAP + \
	100 * (SMALL_MAX_ALLOC_SIZE + SIZEOF_T_BLOCK), getpagesize()))

extern void 			*global_start;
extern pthread_mutex_t	malloc_mtx;

size_t					align(size_t nb);
size_t					align_on(size_t nb, size_t alignment);
void					link_nodes(t_node *n1, t_node *n2);
void					print_base(uintptr_t nb, unsigned int base);

void					insert_after_node(t_node *node, t_node *new);
void					*malloc_impl(size_t size);
void					free_impl(void *ptr);
void					show_alloc_mem_impl();

void					free(void *ptr);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					show_alloc_mem();


#endif