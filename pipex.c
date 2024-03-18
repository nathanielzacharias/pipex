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
	char	**indv_paths;
	char	*tmp;
	char *pathwcmd;
	int access_check;

	pathvar = get_pathvar();
	indv_paths = ft_split(pathvar, ':');
	ssize_t	i;
	i = -1;
	while (indv_paths[++i])
	{
		pathwcmd = ft_strjoin(ft_strjoin(indv_paths[i], "/"), cmd);
		access_check = access(pathwcmd, X_OK);
		if(access_check == 0)
		{	
			tmp = pathwcmd;
			free(indv_paths);
			// free(pathwcmd);
			return (tmp);
		}
		free(pathwcmd);
	}
	free(indv_paths);
	return(NULL);
}

int	 parse_pipex(char *cmd, int pid, int fd, int pipe_end)
{
	char	**cmd_args;
	int 	checkdupchild;
	int 	checkdupparent;
	char	*binpath;
	
	cmd_args = ft_split(cmd, ' ');
	binpath = access_ok(cmd_args[0]);
	// printf("pid is : %d \n", pid);
	if (!binpath)
		return (errno = EACCES, perror("access() denied"), 1);
	// if (pid ==1) printf("binpath is: %s\n", binpath );
	
	if (pid == 0)
	{
		checkdupchild = dup2(pipe_end, STDOUT_FILENO);
		if (checkdupchild < 0) return(perror("dup2() failed in child"), 1);
		execve(binpath, cmd_args, (char *const *)environ);
		// perror("In child: ");
	}
	else
	{
		checkdupparent = dup2(pipe_end, STDIN_FILENO);
		// close(pipe_end);
		if (checkdupparent < 0 ) return(perror("dup2() failed in parent"), 1);
		execve(binpath, cmd_args, (char *const *)environ);
		perror("In parent second child: ");
	}
	free(cmd_args);
	return(0);
}

// int hasErrors(int ac, char *av[], int pipefd[])
// {

// }

int	main(int ac, char *av[])
{
	int in_fd;
	int out_fd;
	int	pipefd[2];
	int	pid;
	int pipe_success;

	// if hasErrors()
	if (ac != 5)
		return(errno = EINVAL, perror("Program requires 5 args"), 1);
	pipe_success = pipe(pipefd);
	if (pipe_success < 0)
		return (errno = EPIPE, perror("pipe_success < 0"), 1);
	in_fd = open(av[1], O_RDONLY);
	out_fd = open(av[ac -1], O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (in_fd < 0 || out_fd < 0)
		return(errno = EBADFD, perror("open() returns -1 for in_fd or out_fd"), 1);
	
	printf("in main before fork\n");

	pid = fork();
	if (pid < 0)
		return(errno = ESRCH, perror("pid < 0"), 1);

	if (pid == 0)
	{
		printf("in first child\n");
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
		close(pipefd[0]);
		parse_pipex(av[2], 0, STDIN_FILENO, pipefd[1]);
	}

	else
	{
		waitpid(pid, NULL, 0);
		printf("in main parent\n");fflush(NULL);
		pid = fork();
		if (pid < 0)
			return(errno = ESRCH, perror("pid < 0"), 1);
		if (pid == 0)
		{
			printf("in main parent after fork in child\n");fflush(NULL);
			dup2(out_fd, STDOUT_FILENO);
			close(out_fd);
			close(pipefd[1]);
			parse_pipex(av[3], 1, STDOUT_FILENO, pipefd[0]);
		}
	}

	// waitpid(pid, NULL, 0);
	// printf("in main parent after fork after child\n");fflush(NULL);
	return(0);
}