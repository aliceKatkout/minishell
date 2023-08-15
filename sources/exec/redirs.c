/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avedrenn <avedrenn@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:16:50 by avedrenn          #+#    #+#             */
/*   Updated: 2023/08/15 16:11:40 by avedrenn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	make_redirs(t_data *d, t_command *cmd)
{
	int	i;

	i = 0;
	d->errnum = 0;
	cmd->fd_in = 0;
	cmd->fd_out = 1;
	if (!cmd->redirs && !cmd->heredocs)
		return (0);
	if (cmd->heredocs && heredoc(cmd, d->env))
		return (EXIT_FAILURE);
	cmd->fds = gc_lstnew(NULL);
	while (cmd->redirs && cmd->redirs[i])
	{
		d->errnum = make_dups_list(cmd, cmd->redirs[i]);
		if (d->errnum != 0)
			return (d->errnum);
		i ++;
	}
	return (d->errnum);
}

int	open_file_fd(t_redir *r, char mode)
{
	int	new_fd;

	new_fd = -1;
	if (r->file && (mode == 'r' || mode == 'b'))
		new_fd = open(r->file, O_RDONLY);
	else if (r->file && mode == 'w')
		new_fd = open(r->file, O_CREAT | O_RDWR | O_TRUNC, 0000644);
	else if (r->file && mode == 'a')
		new_fd = open(r->file, O_CREAT | O_RDWR | O_APPEND, 0000644);
	if (new_fd < 0)
	{
		ft_dprintf(2, "file : %s : %s\n", r->file, strerror(errno));
		return (errno);
	}
	return (new_fd);
}
