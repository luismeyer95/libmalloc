#include <libft_malloc.h>

void fetch_debug_flags(t_debug_flags *flags)
{
	if (getenv("MallocStackLogging"))
		flags->STACK_LOGGING = 1;
	if (getenv("MallocScribble"))
		flags->SCRIBBLE = 1;
}

__attribute__((always_inline))
void *thread_backtrace(void *null)
{
	void *array[10];
	int size;

	(void)null;

	size = backtrace(array, 10);
	backtrace_symbols_fd(array, size, 1);
	return NULL;
}

__attribute__((always_inline))
void log_backtrace(void *alloc)
{
	(void)alloc;

	pthread_t th;
	pthread_create(&th, NULL, thread_backtrace, NULL);
	pthread_join(th, NULL);
}