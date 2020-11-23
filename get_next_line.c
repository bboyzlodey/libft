/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asybil <asybil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 14:16:27 by asybil            #+#    #+#             */
/*   Updated: 2019/11/28 18:27:12 by asybil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char				*ft_strjoindel(char *s1, char *s2)
{
	char	*new;

	new = ft_strjoin(s1, s2);
	free(s1);
	s1 = NULL;
	ft_strclr(s2);
	return (new);
}

static void				ft_lstde(struct s_list *tmp)
{
	free(tmp->content);
	tmp->content = NULL;
	free(tmp);
	tmp = NULL;
}

static struct s_list	*ft_lstsearch(struct s_list *s, char fd)
{
	struct s_list	*tmp;

	tmp = s;
	while (tmp)
	{
		if ((char)tmp->content_size == fd)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

static int				e(int f, struct s_list **h, struct s_list **t, char **l)
{
	int		r;
	char	buf[BUFF_SIZE + 1];
	char	*test;

	ft_bzero(buf, BUFF_SIZE + 1);
	test = NULL;
	r = read(f, buf, BUFF_SIZE);
	while ((*buf || r) && !(test = ft_memset(ft_strchr(buf, '\n'), '\0', 1)))
	{
		*l = ft_strjoindel(*l, buf);
		r = read(f, buf, BUFF_SIZE);
	}
	if (!(*l = ft_strjoindel(*l, buf)))
		*l = ft_strnew(1);
	if (test)
	{
		if ((*t)->content)
			ft_strdel((char**)&(*t)->content);
		(*t)->content = ft_strjoindel(ft_strnew(1), test + 1);
		return (1);
	}
	*h = (*t)->next;
	(*t)->content_size = 0;
	ft_lstde(*t);
	return ((test || r || **l) ? 1 : 0);
}

int						get_next_line(const int fd, char **line)
{
	static struct s_list	*head;
	struct s_list			*tmp;
	char					*test;

	if (fd < 0 || !line || read(fd, "", 0) == -1)
		return (-1);
	test = NULL;
	if (!(head))
		head = ft_lstnew(ft_strnew(1), fd);
	if (!(tmp = ft_lstsearch(head, fd)))
		ft_lstadd(&head, (tmp = ft_lstnew(ft_strnew(1), fd)));
	test = ft_memset(ft_strchr(tmp->content, '\n'), '\0', 1);
	*line = ft_strjoindel(ft_strnew(1), tmp->content);
	if (test)
	{
		tmp->content = ft_strjoindel(tmp->content, test + 1);
		if (!*line)
			*line = ft_strnew(1);
		return (1);
	}
	else if (tmp->content && *((char*)tmp->content))
		ft_lstde(tmp);
	return (e(fd, &head, &tmp, line));
}
