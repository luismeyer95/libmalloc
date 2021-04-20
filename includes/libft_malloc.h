#ifndef LIBFT_MALLOC_H
# define LIBFT_MALLOC_H

# define _GNU_SOURCE

# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <stdbool.h>
# include <libft.h>
# include <execinfo.h>

// remove later
# include <string.h>
# include <errno.h>
# include <stdlib.h>

# define ARENA_COUNT 8

# define LOGFILE_PATH "/tmp/malloc.log"

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


# define LIBMALLOC_LOCK_WRAP(st) \
	pthread_mutex_lock(&malloc_mtx);\
	st;\
	pthread_mutex_unlock(&malloc_mtx)


typedef	enum	s_group
{
	TINY, SMALL, LARGE
}				t_group;

typedef	struct		s_node
{
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

typedef struct		s_arena
{
	int				initialized;
	void			*heap_lst;
	pthread_mutex_t arena_mtx;
}					t_arena;

typedef	struct		s_heap
{
	struct s_heap	*prev;
	struct s_heap	*next;
	size_t			size;
	t_group			group;
	t_arena			*arena;
}					t_heap;

typedef	struct		s_block
{
	struct s_block	*prev;
	struct s_block	*next;
	size_t			size;
	bool 			allocated;
}					t_block;

typedef struct		s_debug_flags
{
	uint8_t			STACK_LOGGING : 1;
	uint8_t			SCRIBBLE : 1;
	uint8_t			PRESCRIBBLE : 1;
	uint8_t			CHECK_HEAP_START : 1;
	uint8_t			CHECK_HEAP_EACH : 1;
}					t_debug_flags;

typedef struct		s_ctl
{
	pthread_key_t	*arena_key;
	pthread_key_t	*recursion_key;
	t_debug_flags	dbg_flags;
}					t_ctl;


extern pthread_mutex_t	malloc_mtx;
extern t_arena			arenas[ARENA_COUNT];

size_t					align(size_t nb);
size_t					align_on(size_t nb, size_t alignment);
void					fetch_debug_flags();
void					try_init_state();

t_ctl					*malloc_ctl();
void					*get_arena();
void					set_arena(void *value);
void					*get_recursive_flag();
void					set_recursive_flag(void *value);

t_arena					*lock_arena();
void					unlock_arena(t_arena *arena);
void					foreach_arena_mutex(int (*lockfunc)(pthread_mutex_t*));
t_arena					*trylock_and_assign_arena();
void					init_arenas(t_arena *arenas);

void					log_backtrace(int fd);
void					log_malloc_call(const char *filepath, size_t size, void *alloc);
void					log_free_call(const char *filepath, void *alloc);

void					link_nodes(t_node *n1, t_node *n2);
void					print_base(int fd, uintptr_t nb, unsigned int base);
void					print_str(int fd, char *str);

void					insert_after_node(t_node *node, t_node *new);
void					*malloc_impl(size_t size, t_arena *arena);
void					free_impl(void *ptr, t_arena *arena);
void					*realloc_impl(void *ptr, size_t size, t_arena *arena);
void					show_alloc_mem_impl();

void					free(void *ptr);
void					*malloc(size_t size);
void					*realloc(void *ptr, size_t size);
void					*calloc(size_t count, size_t size);
void					show_alloc_mem();


#endif