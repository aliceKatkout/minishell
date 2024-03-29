/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dups.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avedrenn <avedrenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 18:10:51 by avedrenn          #+#    #+#             */
/*   Updated: 2023/07/27 16:18:45 by avedrenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_in_out(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
		dupnclose(fd_in, STDIN_FILENO);
	if (fd_out != STDOUT_FILENO)
		dupnclose(fd_out, STDOUT_FILENO);
}

void	dup_pipe(t_data *d)
{
	if (d->index != d->cmds_nb - 1 && !d->cmds[d->index]->last)
		dup2(d->p[1], STDOUT_FILENO);
	if (d->index != 0)
		dupnclose(d->prev_pipe, STDIN_FILENO);
	safe_close(d->p[0]);
	safe_close(d->p[1]);
}

void	dupnclose(int fd1, int fd2)
{
	dup2(fd1, fd2);
	safe_close(fd1);
}
