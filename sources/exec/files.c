/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avedrenn <avedrenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 11:33:28 by avedrenn          #+#    #+#             */
/*   Updated: 2023/08/02 17:46:35 by avedrenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_infile(t_command *c, t_redir *r)
{
	if (c->fd_in != 0)
		safe_close(c->fd_in);
	if (r->file)
	{
		ft_dprintf(2, "infile : %s : %s\n", r->file, strerror(errno));
		ft_dprintf(2, "This is the infile before: %d\n", c->fd_in);

		c->fd_in = open(r->file, O_RDONLY);
		ft_dprintf(2, "This is the infile after: %d\n", c->fd_in);
		if (c->fd_in < 0)
		{
			//c->fd_in = 0;
			ft_dprintf(2, "infile : %s : %s\n", r->file, strerror(errno));
			return (errno);
		}
	}
	else
		c->fd_in = 0;
	return (0);
}

int	get_outfile(t_command *c, t_redir *r, const int mode)
{
	if (c->fd_out != 1)
		safe_close(c->fd_out);
	if (r->file)
	{
		c->fd_out = open(r->file, O_CREAT | O_RDWR | mode, 0000644);
		if (c->fd_out < 0)
		{
			//c->fd_out = 1;
			ft_dprintf(2, "outfile : %s : %s\n", r->file, strerror(errno));
			return (errno);
		}
	}
	else
		c->fd_out = 1;
	return (0);
}
