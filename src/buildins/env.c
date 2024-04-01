/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgrossma <pgrossma@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 20:47:06 by jgotz             #+#    #+#             */
/*   Updated: 2024/04/01 13:46:05 by pgrossma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_envs(char **envv)
{
	while (*envv)
	{
		printf("%s\n", *envv);
		envv++;
	}
}

char	*get_env(char *env)
{
	return (getenv(env));
}
