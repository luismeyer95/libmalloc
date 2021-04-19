#include <libft_malloc.h>

// void *global_start = NULL;
pthread_key_t	*keyptr = NULL;
t_arena			arenas[ARENA_COUNT];
pthread_mutex_t malloc_mtx = PTHREAD_MUTEX_INITIALIZER;

