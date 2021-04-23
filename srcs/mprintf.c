#include <libft_malloc.h>

typedef struct	s_mprintf_opt
{
	size_t		width;
	int			ljustify;
	char		type;
	int			neg;
	uint64_t	arg;
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

int		count_digits(uint64_t n, int base)
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

static inline size_t buffered_write(int fd, char *str, size_t len, int flush)
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

int parse_opt(va_list *ap, char **str, t_mprintf_opt *opt)
{
	opt->neg = 0;
	if (**str == '-')
	{
		opt->ljustify = 1;
		++(*str);
	}
	else
		opt->ljustify = 0;
	opt->width = atoi_consume(str);
	opt->type = **str;
	++(*str);

	if (!is_any_of(opt->type, "sdxpu"))
		return -1;

	opt->arg = va_arg(*ap, uint64_t);
	return 0;
}

static inline size_t	print_u64_base(int fd, uint64_t nb, uint8_t base)
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
	return buffered_write(fd, ptr, ft_strlen(ptr), 0);
}

size_t print_pad(int fd, size_t count)
{
	size_t ret = 0;
	for (size_t i = 0; i < count; ++i)
		ret += buffered_write(fd, " ", 1, 0);
	return ret;
}

size_t print_num_arg(int fd, t_mprintf_opt *opt, uint8_t base)
{
	size_t ret = 0;

	int neg = opt->neg;
	size_t digit_count = count_digits(opt->arg, base) + neg;
	size_t total_pad = MAX(opt->width, digit_count) - digit_count;
	if (opt->ljustify)
	{
		if (neg)
			ret += buffered_write(fd, "-", 1, 0);
		ret += print_u64_base(fd, opt->arg, base);
		ret += print_pad(fd, total_pad);
	}
	else
	{
		ret += print_pad(fd, total_pad);
		if (neg)
			ret += buffered_write(fd, "-", 1, 0);
		ret += print_u64_base(fd, opt->arg, base);
	}
	return ret;
}

size_t print_str_arg(int fd, t_mprintf_opt *opt)
{
	size_t ret = 0;
	char *str = (char*)opt->arg;
	size_t len = ft_strlen(str);
	size_t total_pad = MAX(opt->width, len) - len;

	if (opt->ljustify)
	{
		ret += buffered_write(fd, str, len, 0);
		ret += print_pad(fd, total_pad);
	}
	else
	{
		ret += print_pad(fd, total_pad);
		ret += buffered_write(fd, str, len, 0);
	}
	return ret;
}

size_t print_ptr_arg(int fd, t_mprintf_opt *opt)
{
	size_t ret = 0;
	size_t digit_count = count_digits(opt->arg, 16) + 2;
	size_t total_pad = MAX(opt->width, digit_count) - digit_count;
	if (opt->ljustify)
	{
		ret += buffered_write(fd, "0x", 2, 0);
		ret += print_u64_base(fd, opt->arg, 16);
		ret += print_pad(fd, total_pad);
	}
	else
	{
		ret += print_pad(fd, total_pad);
		ret += buffered_write(fd, "0x", 2, 0);
		ret += print_u64_base(fd, opt->arg, 16);
	}
	return ret;
}

size_t print_arg(int fd, t_mprintf_opt *opt)
{
	size_t ret = 0;
	switch (opt->type)
	{
		case 's':
			ret += print_str_arg(fd, opt);
			break;
		case 'p':
			ret += print_ptr_arg(fd, opt);
			break;
		case 'd':
			if ((int32_t)opt->arg < 0)
			{
				opt->arg = -(int32_t)opt->arg;
				opt->neg = 1;
			}
		case 'u':
			ret += print_num_arg(fd, opt, 10);
			break;
		case 'x':
			ret += print_num_arg(fd, opt, 16);
			break;
		default:
			break;
	}
	return ret;
}

size_t mprintf(int fd, char *str, ...)
{
	size_t ret = 0;
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
			opt_ret = parse_opt(&ap, &str, &opt);
			if (!opt_ret)
				ret += print_arg(fd, &opt);
		}
		else
		{
			ret += buffered_write(fd, str, ft_strlen(str), 1);
			return ret;
		}
	}
	ret += buffered_write(fd, NULL, 0, 1);
	return ret;
}