/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_one.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avedrenn <avedrenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 17:00:40 by avedrenn          #+#    #+#             */
/*   Updated: 2023/08/15 15:55:40 by avedrenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_one(t_data *d)
{
	d->index = 1;
	if (make_redirs(d, d->cmds[0]) != 0)
		return(d->errnum);
	if (d->cmds[0]->command && is_builtin(d->cmds[0], d))
		exec_builtin_parent(d, d->cmds[0]);
	else if (d->cmds[0]->command)
	{
		d->pid[0] = fork();
		reinit_sigs();
		if (d->pid[0] < 0)
			ft_dprintf(2, "error : %s", strerror(errno));
		else if (d->pid[0] == 0)
		{
			signal(SIGQUIT, SIG_DFL);
			//save_state(1);
			exec_w_execve(d, d->cmds[0]);
			close_list(d->cmds[0]->fds);
		}
	}
	if (g_termsig)
		keep_exit_status(g_termsig);
	safe_close(d->cmds[0]->fd_in);
	safe_close(d->cmds[0]->fd_out);
	//save_stds('r');
	return (d->errnum);
}

int	exec_builtin_parent(t_data *d, t_command *cmd)
{
	if (strcmp(cmd->command, "exit"))
	{
		d->save_in = dup(STDIN_FILENO);
		d->save_out = dup(STDOUT_FILENO);
	}
	dup_in_out(cmd->fd_in, cmd->fd_out);
	dup_list(cmd->fds);
	keep_exit_status(exec_builtin(cmd, d));
	close_list(cmd->fds);
	dupnclose(d->save_in, STDIN_FILENO);
	dupnclose(d->save_out, STDOUT_FILENO);
	return (0);
}

void	exec_w_execve(t_data *d, t_command *cmd)
{
	int errnum;

	/* if (d->env->is_empty)
	{
		ft_dprintf(2, "env : no env = no command\n");
		exit_free_gc(127);
	} */
	dup_in_out(cmd->fd_in, cmd->fd_out);
	dup_list(cmd->fds);
	errnum = check_path(cmd, d->env);
	save_stds('c');
	if (errnum)
	{
		if (errnum == 127)
			ft_dprintf(2, "%s : Command not found\n", cmd->command);
		else if (errnum == 126)
			ft_dprintf(2, "%s : Is a directory\n", cmd->command);
		else
			ft_dprintf(2, "%s : %s\n", cmd->command, strerror(errno));
		close_list(cmd->fds);
		exit_free_gc(errnum);
	}
	errnum = execve(cmd->path,
			(char *const *)make_command(cmd), envlist_to_arr(d->env->list_env));
	if (errnum)
	{
		ft_dprintf(2, "%s : %s\n", cmd->command, strerror(errno));
		close_list(cmd->fds);
		exit_free_gc(126);
	}
}
