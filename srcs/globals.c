#include <libft_malloc.h>

t_arena			arenas[ARENA_COUNT];
pthread_mutex_t malloc_mtx = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
// pthread_mutex_t malloc_mtx = PTHREAD_MUTEX_INITIALIZER;

