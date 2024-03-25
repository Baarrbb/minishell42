#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_bzero(void *s, size_t n)
{
	char	*str;
	size_t	i;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ret;

	if (size > 0 && nmemb > (SIZE_MAX / size))
		return (NULL);
	ret = malloc(nmemb * size);
	if (!ret)
		return (NULL);
	ft_bzero(ret, nmemb * size);
	return (ret);
}


char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len || !len)
	{
		ret = (char *)ft_calloc(1, sizeof(char));
		if (!ret)
			return (NULL);
		return (ret);
	}
	if (len >= s_len)
		len = s_len - start;
	else if (len > s_len - start)
		len--;
	ret = (char *)ft_calloc((len + 1), sizeof(char));
	if (!ret)
		return (NULL);
	s += start + (len - 1);
	ret += len;
	while (len--)
		*--ret = *s--;
	return (ret);
}
