#include <libft_malloc_internals.h>

t_arena			arenas[ARENA_COUNT];
pthread_mutex_t malloc_mtx = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
