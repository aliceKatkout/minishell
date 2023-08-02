/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avedrenn <avedrenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:00:45 by nsainton          #+#    #+#             */
/*   Updated: 2023/08/02 18:24:53 by avedrenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_data(t_data *d)
{
	d->prev_pipe = -1;
	d->index = -1;
	d->save_in = -1;
	d->save_out = -1;
	d->pid = gccalloc(d->cmds_nb, sizeof(int));
	if (!d->pid)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	exec_pipeline(t_data *d)
{
	/* if (save_stds('s'))
		return (EXIT_FAILURE); */
	save_stds('s');
	//ft_dprintf(2, "coucou 1: %d", d->cmds_nb);
	if (set_data(d))
		return (1);
	//ft_dprintf(2, "coucou 2: %d", d->cmds_nb);
	if (d->cmds_nb == 1)
		exec_one(d);
	while (++d->index < d->cmds_nb)
	{
		/*
		if global == SIGINT
			break and wait
		*/

		if (make_redirs(d, d->cmds[d->index]))
		{
			if (d->index != d->cmds_nb - 1)
				d->index ++;
			else
				continue ;
		}
		if (pipe(d->p) == -1)
			ft_dprintf(2, "error : %s", strerror(errno));
		if (d->cmds[d->index]->fd_out != STDOUT_FILENO && d->index != d->cmds_nb - 1)
			d->cmds[d->index]->last = 1;
		d->pid[d->index] = fork();
		reinit_sigs();
		exec_command_in_pipeline(d);
	}
	wait_for_childs(d);
	init_sigs();
	if (d->cmds_nb > 1)
	{
		safe_close(d->p[0]);
		safe_close(d->p[1]);
	}
	if (save_stds('r'))
		return (EXIT_FAILURE);
	return (0);
}

void	exec_command_in_pipeline(t_data *d)
{
	int errnum;

	if (d->pid[d->index] == 0 && d->cmds[d->index]->command)
	{
		signal(SIGQUIT, SIG_DFL);
		//dup_in_out(d->cmds[d->index]->fd_in, d->cmds[d->index]->fd_out);
		dup_list(d->cmds[d->index]->fds);
		dup_pipe(d);
		if (is_builtin(d->cmds[d->index], d) == 1)
		{
			errnum = exec_builtin(d->cmds[d->index], d);
			close_list(d->cmds[d->index]->fds);
			save_stds('c');
			exit_free_gc(errnum);
		}
		else if (!is_builtin(d->cmds[d->index], d))
			exec_w_execve(d, d->cmds[d->index]);
	}
	else if (d->pid[d->index] > 0 && d->cmds[d->index]->command)
	{
		close_used_pipes(d, d->cmds[d->index]);
		if (d->cmds[d->index]->last == 1)
		{
			d->cmds = &d->cmds[d->index + 1];
			d->cmds_nb -= d->index + 1;
			safe_close (d->p[0]);
			safe_close (d->p[1]);
			exec_pipeline(d);
		}
	}
}
