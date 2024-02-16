/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nzachari <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:39:44 by nzachari          #+#    #+#             */
/*   Updated: 2024/02/16 16:09:03 by nzachari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>

int	main(int ac, char *av[])
{
	if (ac != 5)
		return (errno = EINVAL, perror("Too few args"), 1);
//	else return (printf("win"), 0);

	int	pipefd[2];
	pipe(pipefd);
	int rpipe = pipefd[1];
	int wpipe = pipefd[0];
	printf("rpipe is:", rpipe);
	printf("wpipe is:", wpipe);

	int pid = fork();
	if (pid < 0) return (errno = ESRCH, perror("pid < 0"), 1);

	char *infile = av[1];
	char *cmd1 = av[2];
	char *cmd2 = av[3];
	char *outfile = av[4];

	if (pid == 0)
	{
		close(wpipe);
		return (0);
		//ft_parse(infile, cmd1, rpipe, pid);
	}

	else
	{
		wait();
		close(rpipe);
		//ft_parse(outfile, cmd2, wpipe, pid);
	}

}
