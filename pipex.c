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




//parse_pipex(pid, )
char	*binpath;
char	**cmd_flag;
int		fd;

cmd_flag = ft_split(cmd, ' ');


//else pipe_pipex(pid, pipefd, file, cmd)



int	main(int ac, char *av[], char *envp[])
{
	if (ac != 5)
		return (errno = EINVAL, perror("Program requires 5 args"), 1);
//	else return (printf("win"), 0);

	int	pipefd[2];
	pipe(pipefd);

	int rpipe = pipefd[0];
	int wpipe = pipefd[1];
	printf("rpipe is:", rpipe);
	printf("wpipe is:", wpipe);

	int pid;
	pid = fork();
	if (pid < 0) return (errno = ESRCH, perror("pid < 0"), 1);

	char *infile = av[1];
	char *cmd1 = av[2];
	char *cmd2 = av[3];
	char *outfile = av[4];

	if (pid == 0)
	{
		close(rpipe);
		//parse_pipex(infile, cmd1, rpipe, pid);
		return (0);
	}

	//search in $PATH /bin directory for cmd1
	//check access to 

	else
	{
		wait();
		close(wpipe);
		//parse_pipex(outfile, cmd2, wpipe, pid);
	}
}
