#include <libft_malloc_internals.h>

inline size_t align_on(size_t nb, size_t alignment)
{
	return (nb + alignment - 1) & ~(alignment - 1); 
}

inline size_t align(size_t nb)
{
	return align_on(nb, ALIGNMENT);
}

void	*get_arena()
{
	t_ctl *ctl = malloc_ctl();
	return pthread_getspecific(*ctl->arena_key);
}

void	set_arena(void *value)
{
	t_ctl *ctl = malloc_ctl();
	pthread_setspecific(*ctl->arena_key, value);
}

void	*get_recursive_flag()
{
	t_ctl *ctl = malloc_ctl();
	return pthread_getspecific(*ctl->recursion_key);
}

void	set_recursive_flag(void *value)
{
	t_ctl *ctl = malloc_ctl();
	pthread_setspecific(*ctl->recursion_key, value);
}

t_ctl	*malloc_ctl()
{
	static t_ctl ctl;
	return &ctl;
}

