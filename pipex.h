/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzachari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 17:19:56 by nzachari          #+#    #+#             */
/*   Updated: 2024/03/18 17:20:04 by nzachari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include "libft/libft.h"
# include <sys/wait.h>

extern const char	**environ;

typedef struct s_fdstruct
{
	int	in_fd;
	int	out_fd;
	int	pipefd[2];

}	t_fdstruct;

const char	*get_pathvar(void);

#endif