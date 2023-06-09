/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_messages.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsainton <nsainton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 15:43:16 by nsainton          #+#    #+#             */
/*   Updated: 2023/05/22 17:03:28 by nsainton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error(char token)
{
	if (! token)
		ft_printf("%snewline'\n", SERROR);
	else
		ft_printf("%s%c'\n", SERROR, token);
}

void	syntax_errors(char *token)
{
	ft_printf("%s%s'\n", SERROR, token);
}
