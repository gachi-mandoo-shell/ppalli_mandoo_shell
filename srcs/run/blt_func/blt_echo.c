/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkeum <jkeum@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 17:59:37 by jkeum             #+#    #+#             */
/*   Updated: 2021/05/16 17:59:38 by jkeum            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_arg(t_token **token, char **result)
{
	while (*token)
	{
		if ((*token)->type == ARGUMENT)
			*result = ft_strjoin(*result, (*token)->arg, 1);
		if ((*token)->next && (*token)->next->type == ARGUMENT)
		{
			*result = ft_strjoin(*result, " ", 1);
			(*token) = (*token)->next;
		}
		else if ((*token)->next)
			(*token) = (*token)->next;
		else
			break ;
	}
}

int		blt_echo(t_token *token, char ***envp)
{
	char	*result;
	int		new_line;

	result = NULL;
	new_line = 1;
	while (token->type != ARGUMENT)
	{
		if (token->next)
			token = token->next;
		else
			break ;
	}
	if (token->arg && !ft_strcmp(token->arg, "-n"))
	{
		new_line = 0;
		token = token->next;
	}
	echo_arg(&token, &result);
	if (new_line)
		result = ft_strjoin(result, "\n", 1);
	ft_putstr_fd(result, STDOUT_FILENO);
	if (result)
		free(result);
	g_global.exit = 0;
	return (1);
}
