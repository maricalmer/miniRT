/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:04:46 by hruiz-fr          #+#    #+#             */
/*   Updated: 2025/02/22 17:58:15 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "./libft.h"

static int		eol_in_buffer(t_file_gnl *f);
static void		set_index_n_linelength(t_file_gnl *f, int *i, int nb_nodes);
static int		create_n_read_node(t_file_gnl *f, int fd);
static void		fill_line_result(t_file_gnl *f, char *line);

char	*get_next_line(int fd)
/*
With a nested list buffer to avoid copy/paste.
...Not sure it is more efficient at all... 
===> do tests and benchmarking...

Returns a string where the next line was saved.
Returns NULL if no line was found (no EOL or no EOF).

Also (sic!) returns NULL if an malloc or read failure occured.
In this case, the buffer isn't cleared so that the program can try again.

Note on PERFORMANCE BOOST ===> set the BUFFER_SIZE so that sizeof(t_list)
is a multiple of 64 ; use aligned_alloc() instead of malloc()...
*/
{
	static t_file_gnl	f[NB_FILES_MAX];
	char				*result;

	if (fd < 0 || fd >= NB_FILES_MAX)
		return (NULL);
	while (!eol_in_buffer(&f[fd]))
		if (create_n_read_node(&f[fd], fd))
			return (NULL);
	if (f[fd].line_length)
	{
		result = malloc(f[fd].line_length + 1);
		if (result)
			fill_line_result(&f[fd], result);
	}
	else
	{
		free(f[fd].buffer);
		f[fd].buffer = NULL;
		result = NULL;
	}
	return (result);
}

static int	eol_in_buffer(t_file_gnl *f)
/*
Check if they is EOL or EOF in the last node of the buffer;
Simultaneously calculate the line_length as it walk through.

Two cases can occur : (see set_index_n_linelength)
	1/ starts at i = f->i if only one node in buffer;
	2/ starts at i = 0 if more than one node;
*/
{
	t_list_gnl		*last_node;
	int				i;
	int				nb_nodes;

	if (f->buffer == NULL)
		return (0);
	nb_nodes = 1;
	last_node = f->buffer;
	while (last_node->next)
	{
		last_node = last_node->next;
		nb_nodes++;
	}
	set_index_n_linelength(f, &i, nb_nodes);
	while (!(i == f->read || last_node->content[i] == '\n'))
	{
		f->line_length++;
		i++;
	}
	if (i == BUFFER_SIZE)
		return (0);
	if (i != f->read)
		f->line_length++;
	return (1);
}

static void	set_index_n_linelength(t_file_gnl *f, int *i, int nb_nodes)
{
	if (nb_nodes == 1)
	{
		*i = f->i;
		f->line_length = 0;
	}
	else
	{
		*i = 0;
		f->line_length = (nb_nodes - 1) * BUFFER_SIZE - f->i;
	}
}

static int	create_n_read_node(t_file_gnl *f, int fd)
/*
Create a new node and add it at the back of the buffer list; 
Fill the node content with the read function. 
	-> Returns 1 on allocation failure;
	-> Returns 0 on success.
Simultaneously update the f->read.
*/
{
	t_list_gnl		*new_node;
	t_list_gnl		*node;
	ssize_t			read_result;

	new_node = malloc(sizeof(t_list_gnl));
	if (!new_node)
		return (1);
	read_result = read(fd, new_node->content, BUFFER_SIZE);
	if (read_result == -1)
	{
		free(new_node);
		return (1);
	}
	f->read = read_result;
	new_node->next = NULL;
	if (f->buffer)
	{
		node = f->buffer;
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		f->buffer = new_node;
	return (0);
}

static void	fill_line_result(t_file_gnl *f, char *line)
{
	int					counter;
	t_list_gnl			*previous_node;

	counter = 0;
	while (counter < f->line_length)
	{
		line[counter] = f->buffer->content[f->i];
		counter++;
		f->i++;
		if (f->i == BUFFER_SIZE || (f->i == f->read && f->buffer->next == NULL))
		{
			previous_node = f->buffer;
			f->buffer = f->buffer->next;
			free(previous_node);
			f->i = 0;
		}
	}
	line[f->line_length] = '\0';
	f->line_length = 0;
}
