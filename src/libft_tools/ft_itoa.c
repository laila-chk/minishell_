/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waboutzo <waboutzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 17:54:00 by waboutzo          #+#    #+#             */
/*   Updated: 2022/07/31 16:15:32 by waboutzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	*init_array(int size)
{
	int *i;
	int	co;

	i = malloc(sizeof(int) * size);
	co = -1;
	while (++co < size)
		i[co] = 0;
	return (i);
}

static int	intlen(int n)
{
	int	i;

	i = 1;
	while (n / 10)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static char	*negative_n(int n, char *str)
{
	int				i;
	int				s;
	unsigned int	w;

	w = (unsigned int) n * (-1);
	s = intlen(n) + 1;
	i = s;
	str = (char *) malloc((s + 1) * sizeof(char));
	if (!str)
		return (0);
	while (s > 1)
	{
		str[s - 1] = (w % 10) + '0';
		w = w / 10;
		s--;
	}
	str[0] = '-';
	str[i] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	int		i;
	int		s;
	char	*str;

	str = NULL;
	if (n < 0)
	{
		str = negative_n(n, str);
	}
	else
	{
		s = intlen(n);
		i = s;
		str = (char *) malloc((s + 1) * sizeof(char));
		if (!str)
			return (0);
		while (s > 0)
		{
			str[s - 1] = (n % 10) + '0';
			n = n / 10;
			s--;
		}
		str[i] = '\0';
	}
	return (str);
}
