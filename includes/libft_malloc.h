#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <stdbool.h>



typedef	enum s_group
{
	TINY, SMALL, LARGE
}		t_group;


typedef	struct s_heap
{
	struct s_heap	*next;
	struct s_heap	*prev;
	t_group			group;

}		t_heap;

typedef	struct s_block
{
	struct s_block	*next;
	struct s_block	*prev;
	size_t			size;
	bool 			allocated;
}		t_block;

#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

#define SIZEOF_T_HEAP ALIGN(sizeof(t_heap))
#define SIZEOF_T_BLOCK ALIGN(sizeof(t_block))

# define TINY_MAX_ALLOC_SIZE 128
# define SMALL_MAX_ALLOC_SIZE 4096

# define TINY_HEAP_SIZE 

size_t tiny_heap_size();
size_t small_heap_size();

int align(int a);
// void free(void *ptr);
// void *malloc(size_t size);
// void *realloc(void *ptr, size_t size);

#endif