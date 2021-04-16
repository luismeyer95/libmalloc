#include <libft_malloc.h>

void *global_start = NULL;
pthread_mutex_t malloc_mtx = PTHREAD_MUTEX_INITIALIZER;

