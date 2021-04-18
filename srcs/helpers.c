#include <libft_malloc.h>

inline size_t align_on(size_t nb, size_t alignment)
{
	return (nb + alignment - 1) & ~(alignment - 1); 
}

inline size_t align(size_t nb)
{
	return align_on(nb, ALIGNMENT);
}

inline void print_str(char *str)
{
	ft_putstr_fd(str, 1);
}

inline void	print_base(uintptr_t nb, unsigned int base)
{
	static char basestr[17] = "0123456789abcdef";

	if (base > 16 || base < 2)
		return;
	if (nb < base)
		ft_putchar_fd(basestr[nb], 1);
	else
	{
		print_base(nb / base, base);
		ft_putchar_fd(basestr[nb % base], 1);
	}
}