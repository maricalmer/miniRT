/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 18:04:00 by dlemaire          #+#    #+#             */
/*   Updated: 2025/03/11 12:23:28 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# define BUFFER_SIZE 92
# define NB_FILES_MAX 1024

# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include <string.h>
# include <stdarg.h>
# include <stdint.h>
# include <math.h>
# include <float.h>
# include <errno.h>
# include <limits.h>


typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_list_gnl
{
	char				content[BUFFER_SIZE];
	struct s_list_gnl	*next;	
}					t_list_gnl;

typedef struct s_file_gnl
{
	t_list_gnl		*buffer;
	int				i;
	int				read;
	int				line_length;
}					t_file_gnl;

int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_iswhitespace(char c);
int		ft_printf(const char *format, ...);
int		ft_printc(const char c);
int		ft_printd(int nbr);
int		ft_putp(uintptr_t nbr);
int		ft_printp(void *adr);
int		ft_prints(const char *s);
int		ft_printud(unsigned int nbr);
int		ft_printud(unsigned int nbr);
int		ft_printx(unsigned int nbr, char *chars);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_strncmp(const char *str1, const char *str2, size_t len);
int		ft_memcmp(const void *str1, const void *str2, size_t size);
int		ft_atoi(const char *str);
int		ft_lstsize(t_list *lst);
char	*ft_itoa(int n);
char	*ft_strchr(const char *str, int c);
char	*ft_strrchr(const char *str, int c);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strdup(const char *s1);
char	*ft_strmapi(const char *s, char (*f)(unsigned int, char));
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strtrim(const char *s, const char *set);
char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
t_list	*ft_lstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void	*ft_memset(void *p, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_calloc(size_t count, size_t size);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	ft_bzero(void *s, size_t n);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
char	*get_next_line(int fd);
float	ft_strtof(const char *str, char **endptr);
int		ft_strtoi(const char *str, char **endptr);
float	ft_fabs(float num);


#endif
