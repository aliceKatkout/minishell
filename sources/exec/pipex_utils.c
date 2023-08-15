/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avedrenn <avedrenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:01:07 by nsainton          #+#    #+#             */
/*   Updated: 2023/08/15 16:02:16 by avedrenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_status(int status, const int cmds_number, const int rank)
{
	if (WIFEXITED(status))
	{
		printf("exited, status=%d\n", WEXITSTATUS(status)); //Don't forget to remove me
		status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 3)
		{
			if (rank == cmds_number)
				ft_printf("Quit (core dumped)\n");
		}
		else
			ft_printf("killed by signal %d\n", WTERMSIG(status));
		status = WTERMSIG(status) + 128;
	}
	else if (WIFSTOPPED(status))
	{
		//printf("stopped by signal %d\n", WSTOPSIG(status));
		status = WSTOPSIG(status) + 128;
	}
	else if (WIFCONTINUED(status))
		printf("continued\n");
	return (status);
}

void	wait_for_childs(t_data	*d)
{
	int	i;
	int	w;
	int	status;

	i = 0;
	status = 0;
	while (i < d->cmds_nb && d->pid[i])
	{
		
		w = waitpid(d->pid[i], &status, WUNTRACED | WCONTINUED);
		
		if (w == -1)
		{
			if (d->pid[i] == -111)
			keep_exit_status(1);
			i ++;
			continue ;
		}
		i ++;
		status = update_status(status, d->cmds_nb, i);
		keep_exit_status(status);
	}
}

char	**make_command(t_command	*cmd)
{
	char	**full_cmd;
	int		i;
	int		j;

	i = 0;
	j = 0;
	full_cmd = gccalloc(ft_arrlen((void **) cmd->args) + 2, sizeof (char *));
	full_cmd[j++] = gc_strdup(cmd->command);
	i = 0;
	while (cmd->args[i])
	{
		full_cmd[j++] = gc_strdup(cmd->args[i]);
		i ++;
	}
	full_cmd[j] = NULL;
	i = 0;
	return (full_cmd);
}

void	close_used_pipes(t_data *d, t_command *cmd)
{
	
	safe_close(d->p[1]);
	safe_close(d->prev_pipe);
	d->prev_pipe = d->p[0];
	if (cmd->fd_in != STDIN_FILENO)
		safe_close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		safe_close(cmd->fd_out);
}

void	safe_close(int fd)
{
	if (fd > 2)
		close(fd);
}

