/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 23:16:21 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/12 16:45:52 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <limits.h>
# include <stdlib.h>
# include <stdbool.h>
# include <errno.h>

int		ft_atoi(const char *str);
long	ft_atol(const char *str, bool *ok);
int		ft_isdigit(int c);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t count, size_t size);

#endif
