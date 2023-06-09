/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsainton <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 16:47:49 by nsainton          #+#    #+#             */
/*   Updated: 2023/06/22 18:03:20 by nsainton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	commands_exec(t_cchar *line, t_data *data)
{
	t_ncommand *commands;

	if (split_line(line, &commands, data->env))
	{
		ft_printf("Syntax error while splitting line\n");
		keep_exit_status(SYNTAX_ERROR);
		return ;
	}
	data->cmds = get_commands_reference(commands);
	if (! data->cmds)
	{
		ft_printf("No commands in data\n");
		return ;
	}
	ft_printf("Some commands have been found\n");
	data->cmds_nb = tablen(data->cmds, sizeof * data->cmds);
	if (!data->cmds_nb)
		return ;
	exec_pipeline(data);
	data->cmds = NULL;
}
