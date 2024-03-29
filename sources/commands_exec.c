/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsainton <nsainton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:00:27 by nsainton          #+#    #+#             */
/*   Updated: 2023/07/27 16:00:29 by nsainton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	commands_exec(t_cchar *line, t_data *data)
{
	t_ncommand	*commands;

	if (split_line(line, &commands, data->env))
	{
		keep_exit_status(SYNTAX_ERROR);
		return ;
	}
	data->cmds = get_commands_reference(commands);
	if (! data->cmds)
		return ;
	data->cmds_nb = tablen(data->cmds, sizeof * data->cmds);
	if (!data->cmds_nb)
		return ;
	exec_pipeline(data);
	data->cmds = NULL;
}
