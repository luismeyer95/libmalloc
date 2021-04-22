#include <libft_malloc.h>

typedef struct	s_mprintf_opt
{
	int			width;
	int			ljustify;
	char		type;
	void		*arg;
}				t_mprintf_opt;

char *ft_strstr(const char *haystack, const char *needle)
{
	return ft_strnstr(haystack, needle, (size_t)-1);
}

bool is_any_of(char c, char *charset)
{
	if (!charset)
		return false;
	size_t len_charset = ft_strlen(charset);
	for (size_t i = 0; i < len_charset; ++i)
		if (c == charset[i])
			return true;
	return false;
}

int		count_digits(long n, int base)
{
	int digit_count = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		n /= base;
		digit_count++;
	}
	return digit_count;
}

static inline ssize_t buffered_write(int fd, char *str, size_t len, int flush)
{
	static char		buf[256];
	static size_t	head = 0;

	size_t i = 0;
	size_t ret = 0;
	while (i < len)
	{
		for (; head < 256 && i < len; ++head, ++i)
			buf[head] = str[i];
		if (head == 256)
		{
			ret += write(fd, buf, 256);
			head = 0;
		}
	}
	if (flush)
	{
		ret += write(fd, buf, head);
		head = 0;
	}

	return ret;
}

int	atoi_consume(char **str)
{
	int num;
	int sign;
	num = 0;
	sign = 1;
	if (**str == '-')
	{
		sign *= -1;
		(*str)++;
	}
	while (**str >= '0' && **str <= '9')
	{
		num = num * 10 + (int)**str - '0';
		(*str)++;
	}
	return (num * sign);
}

int parse_opt(char **str, t_mprintf_opt *opt)
{
	if (**str == '-')
	{
		opt->ljustify = 1;
		++(*str);
	}
	if (**str >= '0' && **str <= '9')
		opt->width = atoi_consume(str);
	opt->type = **str;
	++(*str);

	if (!is_any_of(opt->type, "sdx"))
		return -1;
	return 0;
}

inline size_t	print_base(int fd, uintptr_t nb, uint8_t base)
{
	static char basestr[17] = "0123456789abcdef";
	if (base > 16 || base < 2)
		return -1;
	char strnb[65];
	strnb[64] = 0;
	char *ptr = strnb + 63;
	while (nb >= base)
	{
		*ptr = basestr[nb % base];
		nb /= base;
		--ptr;
	}
	*ptr = basestr[nb];
	size_t ret = buffered_write(fd, ptr, ft_strlen(ptr), 0);
	return ret;
}

size_t print_pad(int fd, size_t count)
{
	size_t ret = 0;
	for (size_t i = 0; i < count; ++i)
		ret += buffered_write(fd, " ", 1, 0);
	return ret;
}

size_t print_num(int fd, long arg, t_mprintf_opt *opt, uint8_t base)
{
	size_t i = 0;
	size_t ret = 0;
	int neg = arg < 0 ? 1 : 0;
	int digit_count = count_digits(arg, base);

	size_t total_pad = opt->width - digit_count - neg;
	if (opt->ljustify)
	{
		if (neg)
			ret += buffered_write(fd, "-", 1, 0);
		ret += print_base(fd, ABS(arg), base);
		ret += print_pad(fd, total_pad);
	}
	else
	{
		ret += print_pad(fd, total_pad);
		if (neg)
			ret += buffered_write(fd, "-", 1, 0);
		ret += print_base(fd, ABS(arg), base);
	}
}

size_t print_arg(int fd, va_list *ap, t_mprintf_opt *opt)
{
	switch (opt->type)
	{
		case 's':
			break;
		case 'd':
			print_num(fd, va_arg(ap, int), opt, 10);
			break;
		case 'x':
			print_num(fd, va_arg(ap, unsigned int), opt, 16);
			break;
		default:
			break;
	}
}

int mprintf(int fd, char *str, ...)
{
	ssize_t ret = 0;
	va_list ap;
	t_mprintf_opt opt;
	int opt_ret;
	
	va_start(ap, str);
	while (*str)
	{
		char *pct = ft_strstr(str, "%");
		if (pct)
		{
			ret += buffered_write(fd, str, pct - str, 0);
			str = pct + 1;
			opt_ret = parse_opt(&str, &opt);
			if (!opt_ret)
			{
				print_arg(&ap, &opt);
			}
			
			
		}

	}


}