#include <libft_malloc.h>

inline size_t align_on(size_t nb, size_t alignment)
{
	return (nb + alignment - 1) & ~(alignment - 1); 
}

inline size_t align(size_t nb)
{
	return align_on(nb, ALIGNMENT);
}

inline void print_str(int fd, char *str)
{
	ft_putstr_fd(str, fd);
}

inline void	print_base(int fd, uintptr_t nb, unsigned int base)
{
	static char basestr[17] = "0123456789abcdef";
	if (base > 16 || base < 2)
		return;
	char strnb[65];
	strnb[64] = 0;
	char *ptr = strnb + 63;
	for ( ; nb >= base; --ptr)
	{
		*ptr = basestr[nb % base];
		nb /= base;
	}
	*ptr = basestr[nb];
	ft_putstr_fd(ptr, fd);
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

