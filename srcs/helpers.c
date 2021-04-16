#include <libft_malloc.h>

inline size_t align_on(size_t nb, size_t alignment)
{
	return (nb + alignment - 1) & ~(alignment - 1); 
}

inline size_t align(size_t nb)
{
	return align_on(nb, ALIGNMENT);
}