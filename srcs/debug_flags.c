#include <libft_malloc.h>

void fetch_debug_flags()
{
	t_ctl *ctl = malloc_ctl();
	if (getenv("MallocStackLogging"))
		ctl->dbg_flags.STACK_LOGGING = 1;
	if (getenv("MallocScribble"))
		ctl->dbg_flags.SCRIBBLE = 1;
	if (getenv("MallocPreScribble"))
		ctl->dbg_flags.PRESCRIBBLE = 1;
}

void log_malloc_call(const char *filepath, size_t size, void *alloc)
{
	void *recursive = get_recursive_flag();

	pthread_mutex_lock(&malloc_mtx);
	int fd = open(filepath, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		pthread_mutex_unlock(&malloc_mtx);
		return;
	}
	if (recursive)
		mprintf(fd, "\t");
	mprintf(fd, ">> malloc(size=%u) = %p\n", size, alloc);
	if (!recursive)
		log_backtrace(fd);
	else
		mprintf(fd, "\t-- backtrace not available --\n");
	ft_putchar_fd('\n', fd);
	
	close(fd);
	pthread_mutex_unlock(&malloc_mtx);
}

void log_free_call(const char *filepath, void *alloc)
{
	void *recursive = get_recursive_flag();

	pthread_mutex_lock(&malloc_mtx);
	int fd = open(filepath, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		pthread_mutex_unlock(&malloc_mtx);
		return;
	}
	if (recursive)
		mprintf(fd, "\t");
	mprintf(fd, ">> free(ptr=%p)\n", alloc);
	if (!recursive)
		log_backtrace(fd);
	else
		mprintf(fd, "\t-- backtrace not available --\n");
	ft_putchar_fd('\n', fd);

	close(fd);
	pthread_mutex_unlock(&malloc_mtx);
}

void log_backtrace(int fd)
{
	void	*array[100];
	int		size;

	set_recursive_flag((void*)1);
	size = backtrace(array, 100);
	if (size)
		backtrace_symbols_fd(array + 2, size - 2, fd);
	set_recursive_flag((void*)0);
}