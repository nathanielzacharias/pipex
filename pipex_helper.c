/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzachari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:16:07 by nzachari          #+#    #+#             */
/*   Updated: 2024/03/18 17:16:13 by nzachari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

const char	*get_pathvar(void)
{
	ssize_t	i;

	i = -1;
	while (environ[++i])
	{
		if (!ft_strncmp((environ[i]), "PATH=", 5))
		{
			return (ft_strtrim(environ[i], "PATH="));
		}
	}
	return (NULL);
}
