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
# include <sys/wait.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

extern const char **environ;

const char *get_pathvar()
{
	ssize_t	i;
	i = -1;
	while(environ[++i])
	{
		if(!ft_strncmp((environ[i]), "PATH=", 5))
		{
			return(ft_strtrim(environ[i], "PATH="));
		}
	}
	return(NULL);
}

char	*access_ok(char *cmd)
{
	const char	*pathvar;
	pathvar = get_pathvar();
	// printf("\npathvar is:%s", pathvar); fflush(NULL);
	
	char	**indv_paths;
	indv_paths = ft_split(pathvar, ':');

	char	*tmp;

	ssize_t	i;
	i = -1;
	while (indv_paths[++i])
	{
		// printf("\nindv_paths[%zd] is: %s",i, indv_paths[i]); fflush(NULL);

		int access_check;
		access_check = access(indv_paths[i], X_OK);
		if(access_check == 0)
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
	binpath = access_ok(cmd_args[0]);
	if (!binpath)
		return (errno = EACCES, perror("access() denied"), 1);
	printf("\nbinpath is: %s\n", binpath);

	int checkdup01;
	int checkdup02;

	// printf("\nbinpath is: %s", binpath); fflush(NULL);
	
	if (pid == 0)
	{
		checkdup01 = dup2(pipe_end, STDOUT_FILENO);
		// checkdup02 = dup2(fd, STDIN_FILENO);
		// if (checkdup01 < 0 || checkdup02 < 0) return(perror("dup2() failed in child"), 1);
		if (checkdup01 < 0) return(perror("dup2() failed in child"), 1);
		// printf("\ncdup01 is:%d \t cdup02 is: %d", checkdup01, checkdup02); fflush(NULL);
		// printf("\nin parse_pipex, child"); fflush(NULL);
		execve(binpath, cmd_args, (char *const *)environ);
	}
	else
	{
		checkdup01 = dup2(pipe_end, STDIN_FILENO);
		// checkdup02 = dup2(fd, STDOUT);
		if (checkdup01 < 0 ) return(perror("dup2() failed in parent"), 1);
		// printf("\ncdup01 is:%d \t cdup02 is: %d", checkdup01, checkdup02); fflush(NULL);
		// printf("\nin parse_pipex, parent"); fflush(NULL);
		execve(binpath, cmd_args, (char *const *)environ);
	}

	free(cmd_args);
	return(0);
}

int	main(int ac, char *av[])
{
	if (ac != 5)
		return (errno = EINVAL, perror("Program requires 5 args"), 1);

	//open file descriptors from arg 1 and arg last
	int in_fd;
	int out_fd;
	in_fd = open(av[1], O_RDONLY);
	out_fd = open(av[ac -1], O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (in_fd < 0 || out_fd < 0)
		return (errno = EBADFD, perror("open() returns -1 for in_fd or out_fd"), 1);
	// printf("\nin_fd is:%d", in_fd); fflush(NULL);
	// printf("\nout_fd is:%d", out_fd); fflush(NULL);

	//open pipes for read and write
	int	pipefd[2];
	int pipe_success;
	pipe_success = pipe(pipefd);
	if (pipe_success < 0)
		return(errno = EPIPE, perror("pipe_success < 0"), 1);
	// printf("\npipefd 0 is:%d", pipefd[0]); fflush(NULL);
	// printf("\npipefd 1 is:%d", pipefd[1]); fflush(NULL);

	// printf("\npast assigns PID is:%d", getpid());
	// fflush(NULL);

	dup2(in_fd, STDIN_FILENO);
	dup2(out_fd, STDOUT_FILENO);	

	int	pid;
	pid = fork();
	if (pid < 0) return (errno = ESRCH, perror("pid < 0"), 1);
	// printf("\npast fork() PID is:%d", getpid()); fflush(NULL);

	if (pid == 0) //run child
	{
		close(pipefd[0]); //close read end
		// printf("\nin pid child");
		// fflush(NULL);
		// dup2(in_fd, STDIN_FILENO);
		parse_pipex(av[2], pid, STDIN_FILENO, pipefd[1]);
		// return (0);
	}

	else //run parent
	{

		waitpid(-1, NULL, 0);
		close(pipefd[1]); //close write end
		// printf("\nin pid parent");
		// fflush(NULL);
		parse_pipex(av[3], pid, STDOUT_FILENO, pipefd[0]);
	}
}