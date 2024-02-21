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
# include "libft/libft.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern const char **environ;

const char *get_pathvar()
{
	ssize_t	i;
	i = -1;
	while(environ[++i])
		if(!ft_strncmp((environ[i]), "PATH=", 5)) return(environ[i]);
	return(NULL);
}



// //parse_pipex(pid, )
// char	*binpath;
// char	**cmd_flag;
// int		fd;

// cmd_flag = ft_split(cmd, ' ');


// //else pipe_pipex(pid, pipefd, file, cmd)


char	*access_ok(char **cmd)
{
	char	*pathvar;
	pathvar = get_pathvar();
	
	char	**indv_paths;
	indv_paths = ft_split(cmd, ':');

	char	*tmp;

	ssize_t	i;
	i = -1;
	while (indv_paths[++i])
	{
		if(access(indv_paths[i]), X_OK)
		{	
			tmp = indv_paths[i];
			free(indv_paths);
			return (tmp);
		} 
	}
	free(indv_paths);
	return(NULL);
}


int	 parse_pipex(char *cmd, int pid, int fd, int pipe_end)
{

	char	**cmd_args;
	cmd_args = ft_split(cmd, ' ');
	
	char	*binpath;
	binpath = access_ok(cmd_args);
	if (!binpath)
		return (errno = EACCES, perror("access() denied"), 1);

	int checkdup01;
	int checkdup02;

	if (pid == 0)
	{
		checkdup01 = dup2(pipe_end, STDOUT);
		checkdup02 = dup2(fd, STDIN);
		if (checkdup01 < 0 || checkdup02 < 0) return(perror("dup2() failed"), 1);
		execve(binpath, cmd_args, environ);
	}
	else
	{

	}

	free(cmd_args);
	return(0);
}


int	main(int ac, char *av[])
{
	if (ac != 5)
		return (errno = EINVAL, perror("Program requires 5 args"), 1);
//	else return (printf("win"), 0);

	int in_fd;
	int out_fd;
	in_fd = open(av[1], O_RDONLY);
	out_fd = open(av[4], O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (in_fd < 0 || out_fd < 0)
		return (errno = EBADFD, perror("open() returns -1 for in_fd or out_fd"), 1);
	
	int	pipefd[2];
	pipe(pipefd);

	char *cmd1 = av[2];
	char *cmd2 = av[3];

	int rpipe = pipefd[0];
	int wpipe = pipefd[1];
	printf("rpipe is:", rpipe);
	printf("wpipe is:", wpipe);

	int	pid;
	pid = fork();
	if (pid < 0) return (errno = ESRCH, perror("pid < 0"), 1);

	if (pid == 0)
	{
		close(rpipe);
		//parse_pipex(infile, cmd1, rpipe, pid);
		parse_pipex(cmd1, pid, in_fd, wpipe);
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